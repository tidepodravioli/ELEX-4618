#include <CSnakeGame.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui\cvui.h"

CSnakeGame::CSnakeGame(cv::Size canvasSize, int comPort)
{
    m_canvas = cv::Mat::zeros(canvasSize, CV_8UC3);
    m_canvasSize = canvasSize;

    cvui::init(PROJECT_NAME);
    m_port.init_com(comPort);

    reset();
}

void CSnakeGame::gpio()
{
    // if(!m_port.checkPort())
    // {
    //     while(!m_port.findPort());
    // }

    CJoystickPosition * currentPosition;
    bool pass = false;
    while(!pass)
    {
         currentPosition = &m_port.get_analog(pass);
    }
    
    if(direction_op::get_opposite_direction(m_currentDirection) != currentPosition->get_simple_direction())
    {
        m_currentDirection = currentPosition->get_simple_direction();
    }
    
    m_stateS1 = m_port.get_button(0);
    m_stateS2 = m_port.get_button(1);
}

void CSnakeGame::update()
{
    m_resetProgram += m_stateS1;

    if(m_resetProgram)
    {
        reset();
    }

    if(m_stateS2)
    {
        m_currentSnakeColour = static_cast<SNAKE_COLOUR>(((int)m_currentSnakeColour + 1) % 3);
        
        switch(m_currentSnakeColour)
        {
            case SNAKE_RED:
                m_snakeColour = SCL_SNAKE_RED;
                break;
            case SNAKE_GREEN:
                m_snakeColour = SCL_SNAKE_GREEN;
                break;
            case SNAKE_BLUE:
                m_snakeColour = SCL_SNAKE_BLUE;
                break;
        }

        changeLED();
    }

    moveSnake();
}

void CSnakeGame::draw()
{
    m_canvas = cv::Mat::zeros(m_canvasSize, CV_8UC3);
    
    //Render UI
    cvui::beginRow(m_canvas, 10,10, -1, -1, 5);
    cvui::window(200,80, PROJECT_NAME);
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 40, -1, -1, 5);
    stringstream colourLabel;
    colourLabel << "Colour : " << getSnakeColourName();
    cvui::text(colourLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 60, -1, 10, 5);
    if(cvui::button("Reset"))
    {
        m_resetProgram = true;
    }

    if(cvui::button("Exit"))
    {
        m_endProgram = true;
    }
    cvui::endRow();

    // Render snake
    for(int segmentIndex = 0; segmentIndex < m_snakeSegments.size(); segmentIndex++)
    {
        cv::Point segment = m_snakeSegments[segmentIndex];
        cv::Rect pixel(SNAKE_CANVAS_DIVISOR * segment.x, SNAKE_CANVAS_DIVISOR * segment.y, SNAKE_CANVAS_DIVISOR, SNAKE_CANVAS_DIVISOR);
        m_canvas(pixel) = m_snakeColour;
    }

    cvui::imshow(PROJECT_NAME, m_canvas);
}

void CSnakeGame::run()
{
    do
    {
        gpio();
        update();
        draw();

        if(m_endProgram) break;
    }
    while(cv::waitKey(1) != 'q');
}

void CSnakeGame::reset()
{
    m_snakeColour = SCL_SNAKE_RED;
    m_currentSnakeColour = SNAKE_RED;
    changeLED();

    //snake init
    m_snakeSegments.clear();
    for(int segment = 0; segment < 20; segment++)
    {
        m_snakeSegments.push_back(cv::Point((m_canvasSize.width / SNAKE_CANVAS_DIVISOR - 1) / 2 + segment,
            (m_canvasSize.height / SNAKE_CANVAS_DIVISOR - 1) / 2));
    }

    m_currentDirection = JOYSTICK_DIRECTION_CENTER;
    m_resetProgram = false;
}

std::string CSnakeGame::getSnakeColourName()
{
    switch(m_currentSnakeColour)
    {
        case SNAKE_RED:
            return "RED";
            break;
        case SNAKE_GREEN:
            return "GREEN";
            break;
        case SNAKE_BLUE:
            return "BLUE";
            break;
    }
}

void CSnakeGame::changeLED()
{
    m_port.set_data(TYPE_DIGITAL, RED_LED_CH, 0);
    m_port.set_data(TYPE_DIGITAL, GREEN_LED_CH, 0);
    m_port.set_data(TYPE_DIGITAL, BLUE_LED_CH, 0);
    switch(m_currentSnakeColour)
    {
        case SNAKE_RED:
            m_port.set_data(TYPE_DIGITAL, RED_LED_CH, 1);
            break;
        case SNAKE_GREEN:
            m_port.set_data(TYPE_DIGITAL, GREEN_LED_CH, 1);
            break;
        case SNAKE_BLUE:
            m_port.set_data(TYPE_DIGITAL, BLUE_LED_CH, 1);
            break;
    }
}

void CSnakeGame::moveSnake()
{
    cv::Point head = m_snakeSegments.back();
    cv::Point newHead = cv::Point(0,0);
    bool addToSnake = false;
    if(m_currentDirection == JOYSTICK_DIRECTION_NORTH)
    {
        if(head.y > 0)
        {
            newHead = cv::Point(head.x, head.y - 1);
            addToSnake = true;
        }
    }
    else if(m_currentDirection == JOYSTICK_DIRECTION_EAST)
    {
        if(head.x < (m_canvasSize.width / SNAKE_CANVAS_DIVISOR) - 1)
        {
            newHead = cv::Point(head.x + 1, head.y);
            addToSnake = true;
        }
    }
    else if(m_currentDirection == JOYSTICK_DIRECTION_SOUTH)
    {
        if(head.y < (m_canvasSize.height / SNAKE_CANVAS_DIVISOR) - 1)
        {
            newHead = cv::Point(head.x, head.y + 1);
            addToSnake = true;
        }
    }
    else if(m_currentDirection == JOYSTICK_DIRECTION_WEST)
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