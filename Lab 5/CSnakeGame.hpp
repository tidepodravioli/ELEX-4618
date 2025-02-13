#pragma once

#define SCL_SNAKE_RED cv::Scalar(0,0,255) ///< The colour red as a cv::Scalar
#define SCL_SNAKE_GREEN cv::Scalar(0,255,0) ///< The colour green as a cv::Scalar
#define SCL_SNAKE_BLUE cv::Scalar(255,0,0)  ///< The colour blue as a cv::Scalar

#include "CBase4618.hpp"
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <time.h>

#define BUTTON_DEBOUNCE_TIMEOUT 25

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

enum SNAKE_STATUS
{
    SNAKE_DEAD,
    SNAKE_ALIVE,
    SNAKE_APPLE
};

/**
 * @brief An instance of the game Snake, implemented using CBase4618 as the base class
 * 
 */
class CSnakeGame : public CBase4618
{
private:
//============================================= GPIO! =============================================

    /**
     * @brief Direction that the snake is going
     * 
     */
    SNAKE_DIRECTION m_currentDirection;

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

//===================================== SNAKE PROPERTIES! =============================================

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

    cv::Point_<int> m_appleLocation;

    /**
     * @brief The size of the canvas, as given in the constructor.
     * Used by other functions for collision and window math.
     * 
     */
    cv::Size m_canvasSize;

    /**
     * @brief The size of the steps(in pixels) the snake is supposed to take
     * 
     */
    float m_stepSize = SNAKE_CANVAS_DIVISOR;

//============================================= FLAGS! =============================================
    /**
     * @brief Flag for ending the program.
     * Program is ended by run() if this is true
     * 
     */
    bool m_flagEndProgram = false;

    /**
     * @brief Flag for resetting the game.
     * Program is reset by update() if this is true
     * 
     */
    bool m_flagResetProgram = false;

    /**
     * @brief Flag for adding another segment to the snake.
     * Is made true by validateSnake() when the player scores.
     * Is reset by updateSnake()
     * 
     */
    bool m_flagAddToSnake = false;

    bool m_flagGameOver = false;

    bool m_flagApple = false;

    
//============================================= TIMING! =============================================

    chrono::system_clock::time_point m_lastDraw;
    chrono::system_clock::time_point m_drawStartTime;
    chrono::system_clock::time_point m_drawSleepUntil;

    chrono::system_clock::time_point m_nextApple;

    float m_fpsRate = 30;
    float m_updatePeriod = 100;
    
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

//===================================== SNAKE MANIPULATION! =============================================
    /**
     * @brief Moves the snake based on the implied direction of the joystick
     * 
     */
    void updateSnake(bool addToSnake = false);

    /**
     * @brief Validates the new snake head's position to see if it's valid or results in the snake dying
     * 
     * @param newHead The location of the new snake head
     * @return SNAKE_STATUS the current status of the snake
     */
    SNAKE_STATUS validateSnake(cv::Point_<int> newHead);

    /**
     * @brief Gives the direction the snake is supposed to be going based on the position of the joystick
     * 
     * @param input The CJoystickPosition representing the current position of the joystick
     * @param currentDirection The current direction the snake is going
     * @return SNAKE_DIRECTION The new direction the snake should be going
     */
    SNAKE_DIRECTION analogToSnake(CJoystickPosition input, SNAKE_DIRECTION currentDirection);

    /**
     * @brief Returns the opposite direction of the given input direction
     * 
     * @param direction The direction we are looking for the opposite of
     * @return SNAKE_DIRECTION The opposite direction of 'direction'
     */
    SNAKE_DIRECTION getOpposite(SNAKE_DIRECTION direction);

//============================================= MULTITHREADING! =============================================
    /**
     * @brief Mutex preventing update() and draw() from accessing the snake data at the same time
     * Initialized in the constructor.
     * 
     */
    std::mutex * snake_data;

    void run_gpio();
    void run_update();

//============================================= SUPPORTING FUNCTIONS! =============================================
    void render_ui();
    void update_colour();

    void createApple();
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