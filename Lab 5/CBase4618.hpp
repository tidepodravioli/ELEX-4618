#pragma once

#include <stdafx.h>
#include <CControl.h>
#include <CJoystickPosition.hpp>


#define MC_DEFAULT_COM_PORT 6

/**
 * @brief Base class for ELEX4618 games.
 * Includes a serial interface to the microcontroller and cvMat as a window canvas.
 * 
 */
class CBase4618
{
protected:
    CControl m_port;
    cv::Mat m_canvas;

public:    
    virtual void gpio() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual void run() = 0;
};