#include "CControlPi.hpp"

using namespace std;

CControlPi::CControlPi(bool &init)
{
  if(gpioInitialise() < 0)
  {
    init = false;
    return;
  }

  launchpadInit();
  init = true;
  
  auto time_now = std::chrono::system_clock::now();
  for(int button = 0; button < BUTTON_NUM_OF_CHANNELS; button++)
  { 
    m_lastDebounceTimes[button] = time_now;
  }
}

CControlPi::~CControlPi()
{
  gpioTerminate();
  spiClose(m_spiHandle);
}

void CControlPi::launchpadInit()
{
  for(int i = 0; i < COUNT_DIGITAL_IN_PINS; i++)
    gpioSetMode(DIGITAL_IN_PINS[i], PI_INPUT);
  
  for(int i = 0; i < COUNT_DIGITAL_OUT_PINS; i++)
  {
    gpioSetMode(DIGITAL_OUT_PINS[i], PI_OUTPUT);
    gpioWrite(DIGITAL_OUT_PINS[i], OFF_VALUE);
  }

  for(int i = 0; i < COUNT_ANALOG_PINS; i++)
    gpioSetMode(ANALOG_PINS[i], PI_INPUT);
 
  for(int i = 0; i < COUNT_SERVO_PINS; i++)
    gpioSetMode(SERVO_PINS[i], PI_OUTPUT);

  m_spiHandle = spiOpen(0, 200000, 3);
}

void CControlPi::get_data (int gpioChannel, int &result)
{
  result = gpioRead(DIGITAL_IN_PINS[gpioChannel]);
}

void CControlPi::set_data(int gpioChannel, int val, bool pwm)
{
  if((val == 0 || val == 1) && !pwm)
    gpioWrite(DIGITAL_OUT_PINS[gpioChannel], val);
  else
  {
    gpioPWM(DIGITAL_OUT_PINS[gpioChannel], val);
  }
}

void CControlPi::set_servo(int gpioChannel, int val)
{
  const int write_us = (2000/180) * (val) + 500;
  gpioServo(SERVO_PINS[gpioChannel], write_us);
}


CJoystickPosition CControlPi::get_analog()
{
  char cmd1[] = {1, 128, 0}, cmd2[] = {1, 144, 0};
  unsigned char raw1[3], raw2[3];
  int _result1, _result2;
  
  spiXfer(m_spiHandle, cmd1, (char*)raw1, 3);
  spiXfer(m_spiHandle, cmd2, (char*)raw2, 3);

  _result1 = ((raw1[1] & 3) << 8) | raw1[2];
  _result2 = ((raw2[1] & 3) << 8) | raw2[2];

  if(_result1 >= 0 && _result2 >= 0)
  {
    return CJoystickPosition(_result1, _result2);
  }
  else return CJoystickPosition();
}

bool CControlPi::get_button(int channel)
{
  int result = -1;
  get_data(channel, result);

  if(result == 1)
  {
    m_lastDebounceTimes[channel] = chrono::system_clock::now();
    m_buttonActive[channel] = false;
    return false;
  }
  else if(result == 0)
  {
    auto elapsed_time = chrono::system_clock::now() - m_lastDebounceTimes[channel];

    // Undo the comment below to debug the proper debounce time!
    //cout << chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() << endl;

    if(!m_buttonActive[channel] && chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() > BUTTON_DEBOUNCE_TIMEOUT)
    {
      m_buttonActive[channel] = true;
      return true;
    }

    return false;
  }

}