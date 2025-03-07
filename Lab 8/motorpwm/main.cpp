#include <pigpio.h>

int main()
{
    gpioInitialise();

    gpioSetMode(26, PI_OUTPUT);

    do
    {
        gpioPWM(26, 255);
    } while (true);
    
}