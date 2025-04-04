#include "CServo.hpp"

class CMotor : public CServo
{
    private:

    public:
    CMotor(int gpiopin);
    void run();
    void stop();
};