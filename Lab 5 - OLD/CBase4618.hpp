#include <stdafx.h>
#include <CControl.h>
#include <CJoystickPosition.hpp>

#define MC_DEFAULT_COM_PORT 6

/**
 * @brief Gives the direction the snake is moving in.
 * 
 */

    
class CBase4618
{
protected:
    CControl m_port;
    bool m_currentButton;
    cv::Mat m_canvas;

public:    
    virtual void gpio() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual void run() = 0;
};