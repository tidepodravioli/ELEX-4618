#include <..\Common\CGameObject.hpp>
#include <vector>
#include <math.h>

#define CSHIP_VERTICES 4
#define CSHIP_TERMINAL_VELOCITY 5
#define CSHIP_VELOCITY_DAMPING 0.99
#define CSHIP_DAMPING_THRESH 0.1
class CShip : public CGameObject
{
    private:
    const Point2i m_ship_vertices[4] = {{0,30}, {10,0}, {20,30}, {10,20}};
    Point2f m_acceleration;
    float m_angle = 60;

    public:
    CShip();
    CShip(Point initialPosition);
    bool collide(CGameObject &obj) override;
    bool collide_wall(Size board) override;
    bool collide_wall(Size board, Point2i newPosition);
    void move() override;
    void move(Size board);
    void draw(Mat &im) override;
    void hit() override;
    void set_acceleration(Point2f acc) { m_acceleration = acc; }
    Point2i get_pos_head();
    void bounce();


};