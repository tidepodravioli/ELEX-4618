#pragma once

#define SCL_SNAKE_RED cv::Scalar(0,0,255) ///< The colour red as a cv::Scalar
#define SCL_SNAKE_GREEN cv::Scalar(0,255,0) ///< The colour green as a cv::Scalar
#define SCL_SNAKE_BLUE cv::Scalar(255,0,0)  ///< The colour blue as a cv::Scalar

#include "CBase4618.hpp"
#include <conio.h>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>

#define BUTTON_DEBOUNCE_TIMEOUT 50

#define PROJECT_NAME "Lab 4 - Snake V1"

#define SNAKE_CANVAS_DIVISOR 10

#define LED_BRIGHTNESS 20 ///< Analog value to write to each LED (out of 255)

#define TARGET_FRAME_RATE 30
#define THREAD_SLEEP_OFFSET 5

/**
 * @brief Enum for the colours of the snake
 * 
 */
enum SNAKE_COLOUR{
    SNAKE_RED,
    SNAKE_GREEN,
    SNAKE_BLUE
};

enum SNAKE_DIRECTION
{
    SNAKE_DIRECTION_NORTH, 
    SNAKE_DIRECTION_EAST, 
    SNAKE_DIRECTION_SOUTH, 
    SNAKE_DIRECTION_WEST,
    SNAKE_DIRECTION_STOP
};

/**
 * @brief An instance of the game Snake, implemented using CBase4618 as the base class
 * 
 */
class CSnakeGame : public CBase4618
{
private:
    /**
     * @brief Direction that the snake is going
     * 
     */
    SNAKE_DIRECTION m_currentDirection;

    /**
     * @brief The current colour of the snake, as represented as a SNAKE_COLOUR enum
     * 
     */
    SNAKE_COLOUR m_currentSnakeColour = SNAKE_RED;

    /**
     * @brief The colour of the snake represented as actual data
     * 
     */
    cv::Scalar m_snakeColour;

    /**
     * @brief Vector containing all of the different segments of the snake body
     * 
     */
    vector<cv::Point_<int>> m_snakeSegments;

    /**
     * @brief The size of the canvas, as given in the constructor.
     * Used by other functions for collision and window math.
     * 
     */
    cv::Size m_canvasSize;

    /**
     * @brief Flag for ending the program.
     * Program is ended by run() if this is true
     * 
     */
    bool m_endProgram = false;

    /**
     * @brief Flag for resetting the game.
     * Program is reset by update() if this is true
     * 
     */
    bool m_resetProgram = false;

    /**
     * @brief Represents whether or not S1 was pressed
     * 
     */
    bool m_stateS1 = false;

    /**
     * @brief Represents whether or not S2 was pressed
     * 
     */
    bool m_stateS2 = false;

    std::chrono::system_clock::time_point m_lastUpdate;
    std::chrono::system_clock::time_point m_updateStartTime;
    std::chrono::system_clock::time_point m_updateEndTime;
    std::chrono::system_clock::time_point m_sleepUntil;
    float m_fpsRate = 30;
    float m_selectedFPS = 30;
    
    /**
     * @brief Resets the program to its original values at construction
     * 
     */
    void reset();

    /**
     * @brief Gets the name of the snake colour as a string
     * 
     * @return string the name of the colour as a string
     */
    std::string getSnakeColourName();

    /**
     * @brief Updates the colour of the LED indicated on the microcontroller
     * 
     */
    void changeLED();

    /**
     * @brief Moves the snake based on the implied direction of the joystick
     * 
     */
    void moveSnake();

    /**
     * @brief 
     * 
     * @param newHead 
     * @return true 
     * @return false 
     */
    bool validateSnake(cv::Point_<int> newHead);

    SNAKE_DIRECTION analogToSnake(CJoystickPosition input, SNAKE_DIRECTION currentDirection);
    SNAKE_DIRECTION getOpposite(SNAKE_DIRECTION direction);

    std::mutex * snake_data;

    void run_gpio();
    void run_update();
    void render_ui();
    void update_colour();
public:
    /**
     * @brief Construct a new CSnakeGame object
     * 
     * @param canvasSize size of the OpenCV canvas
     * @param comPort default COM port to use
     */
    CSnakeGame(cv::Size canvasSize, int comPort = MC_DEFAULT_COM_PORT);

    /**
     * @brief Updates GPIO specific fields in this class
     * 
     */
    void gpio();

    /**
     * @brief Updates the snake body before it is drawn
     * 
     */
    void update();

    /**
     * @brief Draws the generated image on the screen using OpenCV
     * 
     */
    void draw();

    /**
     * @brief Loops between gpio, update, and draw.
     * Ends program if q is pressed
     */
    void run();
};