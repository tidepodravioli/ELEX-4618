#include "CBase4618.hpp"
#include <vector>

#define PROJECT_NAME "Lab 4 - Snake V1"
#define SNAKE_DEFAULT_POSITIONS { cv::Point(0,0)}

#define SNAKE_CANVAS_DIVISOR 10

enum SNAKE_DIRECTION
{
    SNAKE_DIRECTION_NORTH, 
    SNAKE_DIRECTION_EAST, 
    SNAKE_DIRECTION_SOUTH, 
    SNAKE_DIRECTION_WEST
};


class CSnakeGame : public CBase4618
{
private:
    SNAKE_DIRECTION m_currentDirection;
    vector<cv::Point_<int>> m_snakeSegments = SNAKE_DEFAULT_POSITIONS;
    cv::Size m_canvasSize;
    cv::Scalar m_snakeColour;
    /**
     * @brief Interprets the CJoystickPosition to a direction for the snake to go
     * 
     * @param input The current state of the joystick
     * @param currentDirection The current direction the snake is going
     * @return SNAKE_DIRECTION the equivalent new direction for the snake to go
     */
    SNAKE_DIRECTION analogToSnake(CJoystickPosition input, SNAKE_DIRECTION currentDirection);
    SNAKE_DIRECTION getOpposite(SNAKE_DIRECTION direction);
public:
    CSnakeGame(cv::Size canvasSize, int comPort = MC_DEFAULT_COM_PORT);

    void gpio();
    void update();
    void draw();
    void run();
};