#include <headers\CAsteroid.hpp>

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
}

void CAsteroid::bounce()
{
    _velocity *= -1.01;
    hit();
}