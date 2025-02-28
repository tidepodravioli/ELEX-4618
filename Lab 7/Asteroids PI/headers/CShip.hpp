#include <../../Common/CGameObject.hpp>
#include <vector>
#include <math.h>


/**
 * @def CSHIP_VERTICES
 * @brief Number of vertices defining the shape of the spaceship.
 */
#define CSHIP_VERTICES 4

/**
 * @def CSHIP_TERMINAL_VELOCITY
 * @brief Maximum velocity magnitude the spaceship can achieve.
 */
#define CSHIP_TERMINAL_VELOCITY 5

/**
 * @def CSHIP_VELOCITY_DAMPING
 * @brief Damping factor applied to the spaceship's velocity over time.
 */
#define CSHIP_VELOCITY_DAMPING 0.99

/**
 * @def CSHIP_DAMPING_THRESH
 * @brief Velocity threshold below which damping is no longer applied.
 */
#define CSHIP_DAMPING_THRESH 0.1

/**
 * @class CShip
 * @brief Represents a spaceship in the game, inheriting from CGameObject.
 *
 * The CShip class defines the behavior and properties of a spaceship, including
 * its shape, movement, collision detection, and rendering.
 */
class CShip : public CGameObject
{
private:
    /**
     * @brief Vertices defining the shape of the spaceship.
     *
     * The spaceship is represented as a polygon with 4 vertices.
     */
    const Point2i m_ship_vertices[CSHIP_VERTICES] = {{0, 30}, {10, 0}, {20, 30}, {10, 20}};

    /**
     * @brief Current acceleration of the spaceship.
     */
    Point2f m_acceleration;

    /**
     * @brief Current angle of the spaceship (in degrees).
     */
    float m_angle = 60;

public:
    /**
     * @brief Default constructor for CShip.
     */
    CShip();

    /**
     * @brief Constructor for CShip with an initial position.
     * @param initialPosition The initial position of the spaceship.
     */
    CShip(Point initialPosition);

    /**
     * @brief Checks for collision with another game object.
     * @param obj The game object to check for collision.
     * @return True if a collision is detected, false otherwise.
     */
    bool collide(CGameObject &obj) override;

    /**
     * @brief Checks for collision with the game board walls.
     * @param board The size of the game board.
     * @return True if a collision with the wall is detected, false otherwise.
     */
    bool collide_wall(Size board) override;

    /**
     * @brief Checks for collision with the game board walls at a new position.
     * @param board The size of the game board.
     * @param newPosition The position to check for collision.
     * @return True if a collision with the wall is detected, false otherwise.
     */
    bool collide_wall(Size board, Point2i newPosition);

    /**
     * @brief Updates the spaceship's position based on its velocity and acceleration.
     */
    void move() override;

    /**
     * @brief Updates the spaceship's position within the bounds of the game board.
     * @param board The size of the game board.
     */
    void move(Size board);

    /**
     * @brief Draws the spaceship on the provided image.
     * @param im The image (Mat) on which to draw the spaceship.
     */
    void draw(Mat &im) override;

    /**
     * @brief Handles the spaceship being hit (e.g., by a projectile).
     */
    void hit() override;

    /**
     * @brief Sets the spaceship's acceleration.
     * @param acc The new acceleration value.
     */
    void set_acceleration(Point2f acc) { m_acceleration = acc; }

    /**
     * @brief Gets the position of the spaceship's head (front vertex).
     * @return The position of the spaceship's head.
     */
    Point2i get_pos_head();

    /**
     * @brief Handles the spaceship bouncing off a surface (e.g., after a collision).
     */
    void bounce();
};