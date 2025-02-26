#include <../Common/CGameObject.hpp>

#define CMISSLE_UPSPEED -5 ///< The y velocity that the missle will travel at
#define CMISSLE_RADIUS 3 ///< The radius of a missle

/**
 * @brief An Missle object, based on CGameObject
 * 
 */
class CMissle : public CGameObject
{
    private:

    public:
    /**
     * @brief Construct a new CMissle object
     * 
     */
    CMissle();
};