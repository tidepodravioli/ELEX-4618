#include "pigpio.h"

class CServo
{
    private:
    int m_gpiopin;
    void write(int microseconds);


    public:
    CServo(int gpiopin);
    void set(int degrees);
};