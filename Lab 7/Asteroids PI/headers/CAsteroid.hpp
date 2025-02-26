#include <..\Common\CGameObject.hpp>

#define CASTEROID_MAX_SIZE 20 ///< The maximum size of an asteroid (in pixel radius)
#define CASTEROID_MIN_SIZE 10 ///< The minimum size of an asteroid (in pixel radius)

/**
 * @brief An Asteroid object, based on CGameObject
 * 
 */
class CAsteroid : public CGameObject
{
    public:
    /**
     * @brief Construct a new CAsteroid object for a window of the given size
     * 
     * @param board the size of the window
     */
    CAsteroid(Size board);

    /**
     * @brief What to do when a hit to the object occurs
     * 
     */
    void hit() override;

    /**
     * @brief Causes the asteroid to bounce on the wall of the window
     * 
     * @param board the size of the window
     */
    void bounce(Size board);
};