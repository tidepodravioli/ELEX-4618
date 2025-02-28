#include <headers/CAsteroid.hpp>

CAsteroid::CAsteroid(Size board)
{
    srand(time(NULL));

    _radius = rand() % (CASTEROID_MAX_SIZE + CASTEROID_MIN_SIZE + 1) + CASTEROID_MIN_SIZE;
    _lives = _radius / 5;

    const int randomX = rand() % board.width + 1;
    const int velX = rand() % 2 - 1;
    const int velY = rand() % 2 + 1;

    _position = Point2i(randomX, _radius);
    _velocity = Point2f(velX, velY);
    _colour = Scalar(255,255,255);
}

void CAsteroid::hit()
{
    _lives--;
    _radius -= 5;
    _velocity *= 1.2;
}

void CAsteroid::bounce(Size board)
{
    const int pX = _position.x;
    const int pY = _position.y;

    const float bounceFactor = 1.2;

    if(pX - _radius <= 0 || pX + _radius >= board.width)
     _velocity.x *= -1 * bounceFactor;
    
     else _velocity.x *= bounceFactor;

    if(pY - _radius <= 0 || pY + _radius >= board.height) _velocity.y *= -1 * bounceFactor;
    else _velocity.y *= bounceFactor;

    hit();
}