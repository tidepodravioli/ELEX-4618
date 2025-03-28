#include <pigpio.h>
#include <CControlPi.hpp>

int main()
{
    bool init = false;
    CControlPi control(init);
    if(init)
    {

        bool bin1 = true;

        while(true)
        {
            if(control.get_button(0))
            {
                if(bin1)
                {
                    control.set_servo(0, 90);
                    bin1 = false;
                }
                else
                {
                    control.set_servo(0, 25);
                    bin1 = true;
                }
            }
        }
        
    }
}
