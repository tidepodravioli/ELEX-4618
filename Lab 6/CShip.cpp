#include <headers/CShip.hpp>

CShip::CShip(){}

CShip::CShip(Point initialPosition)
{
    _position = initialPosition;
    _velocity = Point2f(0,0);
    m_acceleration = Point2f(0,0);
    _colour = Scalar(0,0,255);
    _radius = 5;
    _lives = 10;
}

bool CShip::collide(CGameObject &obj)
{
    const int shipCenterX = m_ship_vertices[1].x + _position.x;
    const int shipCenterY = m_ship_vertices[0].y/2 + _position.y;

    const Point2i shipCenter(shipCenterX, shipCenterY);
    const Point2i distanceVect = shipCenter - obj.get_pos();
    
    const float distanceMag = norm(distanceVect);
    const int hitRadius = obj.get_radius() + _radius;

    if(distanceMag < hitRadius)
        return true;
    else return false;
}

bool CShip::collide_wall(Size board)
{
    return collide_wall(board, _position);
}

bool CShip::collide_wall(Size board, Point2i newPosition)
{
    const int leftLimit = 0;
    const int rightLimit = board.width - m_ship_vertices[2].x;

    const int topLimit = 0;
    const int bottomLimit = board.height - m_ship_vertices[2].y;

    if(newPosition.x > leftLimit && newPosition.x < rightLimit
        && newPosition.y > topLimit && newPosition.y < bottomLimit)
        return false;
    else return true;
}

void CShip::move()
{
    //do nothing
}

void CShip::move(Size board)
{
    _velocity += m_acceleration;
    const float speed = norm(_velocity);
    const float accMag = norm(m_acceleration);

    if(speed > CSHIP_TERMINAL_VELOCITY)
        _velocity *= (CSHIP_TERMINAL_VELOCITY / speed);
    
    if(abs(m_acceleration.x) < CSHIP_DAMPING_THRESH)
        _velocity.x *= CSHIP_VELOCITY_DAMPING;
    if(abs(m_acceleration.y) < CSHIP_DAMPING_THRESH)
        _velocity.y *= CSHIP_VELOCITY_DAMPING;
    
    Point2i newPosition = _position + (Point2i)_velocity;

    if(!collide_wall(board, newPosition))
        _position = newPosition;
    else
    {
        bool xHit = collide_wall(board, Point2i(newPosition.x, board.height/2));
        bool yHit = collide_wall(board, Point2i(board.width/2, newPosition.y));

        if(xHit && !yHit)
        {
            const int oldX = _position.x;
            const int newY = newPosition.y;

            _position = Point2i(oldX, newY);
        }
        else if(!xHit && yHit)
        {
            const int newX = newPosition.x;
            const int oldY = _position.y;

            _position = Point2i(newX, oldY);
        }
        else
        {
            _position *= -1;
        }
    }
}

void CShip::draw(Mat &im)
{
    std::vector<Point2i> m_translatedShip;

    for(int vertex = 0; vertex < CSHIP_VERTICES; vertex++)
    {
        const int angledX = 1000 * m_ship_vertices[vertex].x + 1000.0 * cos(m_angle * 3.14159/180.0);
        const int angledY = 1000 * m_ship_vertices[vertex].y + 1000.0 * sin(m_angle * 3.14159/180.0);

        m_translatedShip.push_back(Point2i(angledX/1000 + _position.x, angledY/1000 + _position.y));
    }

    polylines(im, m_translatedShip, true, _colour);
}

Point2i CShip::get_pos_head()
{
    return Point2i(_position.x + m_ship_vertices[1].x, _position.y);
}

void CShip::bounce()
{
    _velocity *= -1;
}

void CShip::hit()
{
    _lives--;
    bounce();
}