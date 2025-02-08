#include <CSnakeGame.hpp>

CSnakeGame::CSnakeGame(cv::Size canvasSize, int comPort)
{
    m_canvas = cv::Mat::zeros(canvasSize, CV_8UC3);
    m_canvasSize = canvasSize;
    m_snakeColour = cv::Scalar(0,0,255);

    //snake init
    for(int segment = 1; segment <= 20; segment++)
    {
        m_snakeSegments.push_back(cv::Point(segment, 0));
    }

    m_port.init_com(comPort);
    m_currentDirection = SNAKE_DIRECTION_NORTH;
    m_currentButton = false;
}

void CSnakeGame::gpio()
{
    if(!m_port.checkPort())
    {
        while(!m_port.checkPort());
    }

    CJoystickPosition currentPosition;
    bool pass = false;
    while(!pass)
    {
         currentPosition = m_port.get_analog(pass);
    }
    SNAKE_DIRECTION newDirection = analogToSnake(currentPosition, m_currentDirection);
    if(newDirection != getOpposite(m_currentDirection))
        m_currentDirection = newDirection;
    
    while(!pass)
    {
        m_currentButton = m_port.get_button(pass);
    }
}



void CSnakeGame::run()
{
    while(true)
    {
        gpio();
        update();
        draw();
    }
}

void CSnakeGame::update()
{
    cv::Point head = m_snakeSegments.back();
    cv::Point newHead = cv::Point(0,0);
    bool addToSnake = false;
    if(m_currentDirection == SNAKE_DIRECTION_NORTH)
    {
        if(head.y > 0)
        {
            newHead = cv::Point(head.x, head.y - 1);
            addToSnake = true;
        }
    }
    else if(m_currentDirection == SNAKE_DIRECTION_EAST)
    {
        if(head.x < (m_canvasSize.width / SNAKE_CANVAS_DIVISOR) - 1)
        {
            newHead = cv::Point(head.x + 1, head.y);
            addToSnake = true;
        }
    }
    else if(m_currentDirection == SNAKE_DIRECTION_SOUTH)
    {
        if(head.y < (m_canvasSize.height / SNAKE_CANVAS_DIVISOR) - 1)
        {
            newHead = cv::Point(head.x, head.y + 1);
            addToSnake = true;
        }
    }
    else if(m_currentDirection == SNAKE_DIRECTION_WEST)
    {
        if(head.x > 0)
        {
            newHead = cv::Point(head.x - 1, head.y);
            addToSnake = true;
        }
    }

    if(addToSnake)
    {
        m_snakeSegments.push_back(newHead);
        m_snakeSegments.erase(m_snakeSegments.begin());
    }
}

void CSnakeGame::draw()
{
    m_canvas = cv::Mat::zeros(m_canvasSize, CV_8UC3);
    for(int segmentIndex = 0; segmentIndex < m_snakeSegments.size(); segmentIndex++)
    {
        cv::Point segment = m_snakeSegments[segmentIndex];
        cv::Rect pixel(SNAKE_CANVAS_DIVISOR * segment.x, SNAKE_CANVAS_DIVISOR * segment.y, SNAKE_CANVAS_DIVISOR, SNAKE_CANVAS_DIVISOR);
        m_canvas(pixel) = m_snakeColour;
    }

    cv::imshow(PROJECT_NAME, m_canvas);
    cv::waitKey(1);
}

SNAKE_DIRECTION CSnakeGame::analogToSnake(CJoystickPosition input, SNAKE_DIRECTION currentDirection)
{
    JOYSTICK_DIRECTION joystickDirection = input.get_direction();

    switch(joystickDirection)
    {
        case JOYSTICK_DIRECTION_NORTH:
            return SNAKE_DIRECTION_NORTH;
            break;
        case JOYSTICK_DIRECTION_NORTHEAST:
            return currentDirection;
            break;
        case JOYSTICK_DIRECTION_EAST:
            return SNAKE_DIRECTION_EAST;
            break; 
        case JOYSTICK_DIRECTION_SOUTHEAST:
            return currentDirection;
            break;
        case JOYSTICK_DIRECTION_SOUTH: 
            return SNAKE_DIRECTION_SOUTH;
            break;
        case JOYSTICK_DIRECTION_SOUTHWEST:
            return currentDirection;
            break;
        case JOYSTICK_DIRECTION_WEST:
            return SNAKE_DIRECTION_WEST;
            break;
        case JOYSTICK_DIRECTION_NORTHWEST:
            return currentDirection;
            break;
        case JOYSTICK_DIRECTION_CENTER:
            return currentDirection;
            break;
    }
}

SNAKE_DIRECTION CSnakeGame::getOpposite(SNAKE_DIRECTION direction)
{
    switch(direction)
    {
        case SNAKE_DIRECTION_NORTH:
            return SNAKE_DIRECTION_SOUTH;
            break;
        case SNAKE_DIRECTION_EAST:
            return SNAKE_DIRECTION_WEST;
            break;
        case SNAKE_DIRECTION_SOUTH:
            return SNAKE_DIRECTION_NORTH;
            break;
        case SNAKE_DIRECTION_WEST:
            return SNAKE_DIRECTION_EAST;
            break;
    }
}