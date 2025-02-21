#include <CGameObject.hpp>

void CGameObject::move()
{
    _position = _position + (Point2i)_velocity;
}

bool CGameObject::collide(CGameObject &obj)
{
    const Point distanceVect = _position - obj._position;
    
    const float distance = norm(distanceVect);
    const int collisionRadius = _radius + obj._radius;

    if(distance < collisionRadius)
        return true;
    else return false;
}

bool CGameObject::collide_wall(Size board)
{
    if(_position.x - _radius <= 0)
        return true;
    else if(_position.x + _radius >= board.width)
        return true;
    else if(_position.y - _radius <= 0)
        return true;
    else if(_position.y + _radius >= board.height)
        return true;
    else return false;
}

void CGameObject::hit()
{
    _lives--;
    
}

void CGameObject::draw(Mat &im)
{
    circle(im, _position, _radius, _colour);
}