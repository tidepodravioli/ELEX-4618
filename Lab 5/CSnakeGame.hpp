/**
 * @file CSnakeGame.hpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @version 0.1
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
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

#define BUTTON_DEBOUNCE_TIMEOUT 25 ///< Debounce for the buttons in gpio (in ms)

#define PROJECT_NAME "Lab 5 - Snake V2" ///< The program name to show at the top of the window

#define SNAKE_CANVAS_DIVISOR 10 ///< Default step size at start of game (in pixels)
#define APPLE_REGEN_TIME 5 ///< Time before a new apple is drawn (in seconds)

#define LED_BRIGHTNESS 20 ///< Analog value to write to each LED (out of 255)

#define TARGET_FRAME_RATE 30 ///< Target frame rate for draw() (in fps)
#define THREAD_SLEEP_OFFSET 5 ///< Amount to be short by for time so the thread can yield (in ms)

/**
 * @brief Enum for the colours of the snake
 * 
 */
enum SNAKE_COLOUR{
    SNAKE_RED,
    SNAKE_GREEN,
    SNAKE_BLUE
};

/**
 * @brief Enum for the possible directions the snake can go
 * 
 */
enum SNAKE_DIRECTION
{
    SNAKE_DIRECTION_NORTH, 
    SNAKE_DIRECTION_EAST, 
    SNAKE_DIRECTION_SOUTH, 
    SNAKE_DIRECTION_WEST,
    SNAKE_DIRECTION_STOP
};

/**
 * @brief Enum for the current status of a snake update
 * 
 */
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

    /**
     * @brief Location of the apple on the grid
     * 
     */
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

    int m_lastStepSize = SNAKE_CANVAS_DIVISOR;

    /**
     * @brief The current score recorded by the game
     * 
     */
    int m_snakeScore = 0;

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

    /**
     * @brief Flag that determines whether or not the game has reached an game over condition
     * 
     */
    bool m_flagGameOver = false;

    /**
     * @brief Flag that determines whether or not an apple already exists on the game field
     * 
     */
    bool m_flagApple = false;

    /**
     * @brief Flag that determines whether or not the colour should be updated
     * 
     */
    bool m_flagUpdateColour = true;

    //============================================= TIMING! =============================================
    
    /**
     * @brief The time at the last draw() call
     * 
     */
    chrono::system_clock::time_point m_lastDraw;

    /**
     * @brief The time at the current draw() call
     * 
     */
    chrono::system_clock::time_point m_drawStartTime;

    /**
     * @brief The time that draw will wait until to fix the frame rate
     * 
     */
    chrono::system_clock::time_point m_drawSleepUntil;

    /**
     * @brief The time until the next apple (if one hasn't already been generated)
     * 
     */
    chrono::system_clock::time_point m_nextApple;

    /**
     * @brief The current measured frame rate
     * 
     */
    float m_fpsRate = 30;

    /**
     * @brief The amount of ms each update() cycle is supposed to take
     * 
     */
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

    
    /**
     * @brief Extrapolates the snake from one system of coordinates to another
     * 
     * @param originalStep Original step size to divide the canvas by
     * @param newStep New step size to divide the canvas by
     */
    void extrapolateSnake(int originalStep, int newStep);

    //============================================= MULTITHREADING! =============================================

    /**
     * @brief Mutex preventing update() and draw() from accessing the snake data at the same time
     * Initialized in the constructor.
     * 
     */
    std::mutex * snake_data;

    /**
     * @brief Multithreading for gpio()
     * 
     */
    void run_gpio();

    /**
     * @brief Multithreading for update()
     * 
     */
    void run_update();

    /**
     * @brief Multithreading for draw()
     * 
     */
    void run_draw();

    //============================================= SUPPORTING FUNCTIONS! =============================================

    /**
     * @brief Renders the top left window ui using cvui
     * 
     */
    void render_ui();

    /**
     * @brief Updates the colour to be used by the snake and the microcontroller LED
     * 
     */
    void update_colour();

    /**
     * @brief Generates a new apple if there isn't already one on the field
     * 
     */
    void createApple();

    /**
     * @brief Prints a cv::Point to the console in nice format
     * 
     */
    void print_point(cv::Point_<int> point);
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