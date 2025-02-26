#include <headers/CMissle.hpp>

CMissle::CMissle()
{
    _velocity = Point2f(0, CMISSLE_UPSPEED);
    _radius = CMISSLE_RADIUS;
    _lives = 1;
    _colour = Scalar(0,0,255);
}
