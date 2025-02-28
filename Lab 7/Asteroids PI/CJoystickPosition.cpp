#include "headers/CJoystickPosition.hpp"

CJoystickPosition::CJoystickPosition()
{
    m_joystickx = (JOYSTICK_X_MAX - JOYSTICK_X_MIN) / 2;
    m_joysticky = (JOYSTICK_Y_MAX - JOYSTICK_Y_MIN) / 2;
}

CJoystickPosition::CJoystickPosition(int x, int y)
{
    m_joystickx = x;
    m_joysticky = y;
}
CJoystickPosition::~CJoystickPosition()
{

}

int CJoystickPosition::getX()
{
    return m_joystickx;
}
int CJoystickPosition::getY()
{
    return m_joysticky;
}

int CJoystickPosition::percentX()
{
    int numerator = 100 * (m_joystickx - JOYSTICK_X_MIN);
    int denominator = JOYSTICK_X_MAX - JOYSTICK_X_MIN;

    return numerator/denominator;
}
int CJoystickPosition::percentY()
{
    int numerator = 100 * (m_joysticky - JOYSTICK_Y_MIN);
    int denominator = JOYSTICK_Y_MAX - JOYSTICK_Y_MIN;

    return numerator/denominator;
}

bool CJoystickPosition::withinValues(int input, int min, int max)
{
    return (min <= input && input <= max);
}

bool CJoystickPosition::joystickActive()
{
    const int px = percentX();
    const int py = percentY();

    return (px <= JOYSTICK_IGNORE_LRVP || px >= JOYSTICK_IGNORE_URVP) || (py <= JOYSTICK_IGNORE_LRVP || py >= JOYSTICK_IGNORE_URVP);
}

JOYSTICK_DIRECTION CJoystickPosition::get_direction()
{
    const int px = percentX();
    const int py = percentY();

    if(withinValues(px, JOYSTICK_IGNORE_LRVP, JOYSTICK_IGNORE_URVP) && py >= JOYSTICK_IGNORE_URVP)
        return JOYSTICK_DIRECTION_NORTH;

    else if(px >= JOYSTICK_IGNORE_URVP && py >= JOYSTICK_IGNORE_URVP)
        return JOYSTICK_DIRECTION_NORTHEAST;

    else if(px >= JOYSTICK_IGNORE_URVP && withinValues(py, JOYSTICK_IGNORE_LRVP, JOYSTICK_IGNORE_URVP))
        return JOYSTICK_DIRECTION_EAST;

    else if(px >= JOYSTICK_IGNORE_URVP && py <= JOYSTICK_IGNORE_LRVP)
        return JOYSTICK_DIRECTION_SOUTHEAST;

    else if(withinValues(px, JOYSTICK_IGNORE_LRVP, JOYSTICK_IGNORE_URVP) && py <= JOYSTICK_IGNORE_LRVP)
        return JOYSTICK_DIRECTION_SOUTH;

    else if(px <= JOYSTICK_IGNORE_LRVP && py <= JOYSTICK_IGNORE_LRVP)
        return JOYSTICK_DIRECTION_SOUTHWEST;

    else if(px <= JOYSTICK_IGNORE_LRVP && withinValues(py, JOYSTICK_IGNORE_LRVP, JOYSTICK_IGNORE_URVP))
        return JOYSTICK_DIRECTION_WEST;
        
    else if(px <= JOYSTICK_IGNORE_LRVP && py >= JOYSTICK_IGNORE_URVP)
        return JOYSTICK_DIRECTION_NORTHWEST;
        
    else return JOYSTICK_DIRECTION_CENTER;
}

JOYSTICK_DIRECTION CJoystickPosition::get_opposite_direction()
{
    JOYSTICK_DIRECTION realDirection = get_direction();
    switch(realDirection)
    {
        case JOYSTICK_DIRECTION_NORTH:
            return JOYSTICK_DIRECTION_SOUTH;
            break;
        case JOYSTICK_DIRECTION_NORTHEAST:
            return JOYSTICK_DIRECTION_SOUTHWEST;
            break;
        case JOYSTICK_DIRECTION_EAST:
            return JOYSTICK_DIRECTION_WEST;
            break;
        case JOYSTICK_DIRECTION_SOUTHEAST:
            return JOYSTICK_DIRECTION_NORTHWEST;
            break;
        case JOYSTICK_DIRECTION_SOUTH:
            return JOYSTICK_DIRECTION_NORTH;
            break;
        case JOYSTICK_DIRECTION_SOUTHWEST:
            return JOYSTICK_DIRECTION_NORTHEAST;
            break;
        case JOYSTICK_DIRECTION_WEST:
            return JOYSTICK_DIRECTION_EAST;
            break;
        case JOYSTICK_DIRECTION_NORTHWEST:
            return JOYSTICK_DIRECTION_SOUTHEAST;
            break;
        case JOYSTICK_DIRECTION_CENTER:
            return JOYSTICK_DIRECTION_CENTER;
            break;
    }
}

JOYSTICK_DIRECTION CJoystickPosition::get_simple_direction()
{
    const int px = percentX();
    const int py = percentY();

    if(px < JOYSTICK_IGNORE_LRVP || px > JOYSTICK_IGNORE_URVP || py < JOYSTICK_IGNORE_LRVP || py > JOYSTICK_IGNORE_URVP)
    {
        if(py > px) // NORTH AND WEST
        {
            if(withinValues(px, 12, 86) && py > JOYSTICK_IGNORE_URVP)
                return JOYSTICK_DIRECTION_NORTH;
            else if(withinValues(py, 12, 86) && px < JOYSTICK_IGNORE_LRVP)
                return JOYSTICK_DIRECTION_WEST;
            else return JOYSTICK_DIRECTION_WEST;
        }
        else if (px > py) // EAST AND SOUTH
        {
            if(withinValues(px, 12, 86) && py < JOYSTICK_IGNORE_LRVP)
                return JOYSTICK_DIRECTION_SOUTH;
            else if(withinValues(py, 12, 86) && px > JOYSTICK_IGNORE_URVP)
                return JOYSTICK_DIRECTION_EAST;
            else return JOYSTICK_DIRECTION_EAST;
        }
        return JOYSTICK_DIRECTION_CENTER;
    }
    else
        return JOYSTICK_DIRECTION_CENTER;
}

JOYSTICK_DIRECTION CJoystickPosition::get_simple_opposite()
{
    return direction_op::get_opposite_direction(get_simple_direction());
}


namespace direction_op
{
    JOYSTICK_DIRECTION get_opposite_direction(JOYSTICK_DIRECTION input)
    {
        switch(input)
        {
            case JOYSTICK_DIRECTION_NORTH:
                return JOYSTICK_DIRECTION_SOUTH;
                break;
            case JOYSTICK_DIRECTION_NORTHEAST:
                return JOYSTICK_DIRECTION_SOUTHWEST;
                break;
            case JOYSTICK_DIRECTION_EAST:
                return JOYSTICK_DIRECTION_WEST;
                break;
            case JOYSTICK_DIRECTION_SOUTHEAST:
                return JOYSTICK_DIRECTION_NORTHWEST;
                break;
            case JOYSTICK_DIRECTION_SOUTH:
                return JOYSTICK_DIRECTION_NORTH;
                break;
            case JOYSTICK_DIRECTION_SOUTHWEST:
                return JOYSTICK_DIRECTION_NORTHEAST;
                break;
            case JOYSTICK_DIRECTION_WEST:
                return JOYSTICK_DIRECTION_EAST;
                break;
            case JOYSTICK_DIRECTION_NORTHWEST:
                return JOYSTICK_DIRECTION_SOUTHEAST;
                break;
            case JOYSTICK_DIRECTION_CENTER:
                return JOYSTICK_DIRECTION_CENTER;
                break;
        }
    }

    std::string get_direction_string(JOYSTICK_DIRECTION direction)
    {
        switch(direction)
        {
            case JOYSTICK_DIRECTION_NORTH:
                return "NORTH";
                break;
            case JOYSTICK_DIRECTION_NORTHEAST:
                return "NORTHEAST";
                break;
            case JOYSTICK_DIRECTION_EAST:
                return "EAST";
                break;
            case JOYSTICK_DIRECTION_SOUTHEAST:
                return "SOUTHEAST";
                break;
            case JOYSTICK_DIRECTION_SOUTH:
                return "SOUTH";
                break;
            case JOYSTICK_DIRECTION_SOUTHWEST:
                return "SOUTHWEST";
                break;
            case JOYSTICK_DIRECTION_WEST:
                return "WEST";
                break;
            case JOYSTICK_DIRECTION_NORTHWEST:
                return "NORTHWEST";
                break;
            case JOYSTICK_DIRECTION_CENTER:
                return "CENTER";
                break;
        }
    }
}