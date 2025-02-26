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
}

void CControlPi::launchpadInit()
{
    
}

void CControlPi::get_data (int gpioChannel, int &result)
{
  result = gpioRead(DIGITAL_PINS[gpioChannel]);
}

void CControlPi::set_data(int gpioChannel, int val, bool pwm)
{
  if((val == 0 || val == 1) && !pwm)
    gpioWrite(DIGITAL_PINS[gpioChannel], val);
  else
  {
    gpioPWM(DIGITAL_PINS[gpioChannel], val);
  }
}




CJoystickPosition CControlPi::get_analog()
{
  int _result1, _result2;
  get_data(CH_JOYSTICK_X, _result1);
  get_data(CH_JOYSTICK_Y, _result2);

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