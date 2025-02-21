/**
 * @file CSnakeGame.hpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Represents an instance of the Lab 4 CSnakeGame
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
#include <conio.h>
#include <string>
#include <sstream>
#include <vector>

#define BUTTON_DEBOUNCE_TIMEOUT 50 ///< Button debounce timeout (overrides CControl.h)

#define PROJECT_NAME "Lab 4 - Snake V1" ///< Program title/heading

#define SNAKE_CANVAS_DIVISOR 10 ///< Default step size of snake (in pixels)

#define LED_BRIGHTNESS 20 ///< Analog value to write to each LED (out of 255)

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
    JOYSTICK_DIRECTION m_currentDirection;

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