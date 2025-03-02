#pragma once

#include "CControlPi.hpp"
#include "../Common/stdafx.h"
#include <CJoystickPosition.hpp>


#define MC_DEFAULT_COM_PORT 6

/**
 * @brief Base class for ELEX4618 games.
 * Includes a serial interface to the microcontroller and cvMat as a window canvas.
 * 
 */
class CBase4618Pi
{
protected:
    /**
     * @brief Handles the serial communication between the game and the microcontroller
     * 
     */
    CControlPi * m_port;

    /**
     * @brief Canvas representing the current frame to be displayed on screen by OpenCV
     * 
     */
    cv::Mat m_canvas;

public:    
    /**
     * @brief Updates GPIO specific fields in this class
     * 
     */
    virtual void gpio() = 0;

    /**
     * @brief Updates the snake body before it is drawn
     * 
     */
    virtual void update() = 0;

    /**
     * @brief Draws the generated image on the screen using OpenCV
     * 
     */
    virtual void draw() = 0;

    /**
     * @brief Loops between gpio, update, and draw.
     * Ends program if q is pressed
     */
    virtual void run() = 0;
};