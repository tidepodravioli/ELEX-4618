#include <..\Common\CGameObject.hpp>

#define CASTEROID_MAX_SIZE 20
#define CASTEROID_MIN_SIZE 10
class CAsteroid : public CGameObject
{
    public:
    CAsteroid(Size board);
    void hit() override;
    void bounce();
};