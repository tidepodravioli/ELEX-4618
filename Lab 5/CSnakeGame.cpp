#include <CSnakeGame.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui\cvui.h"

CSnakeGame::CSnakeGame(cv::Size canvasSize, int comPort)
{
    m_canvas = cv::Mat::zeros(canvasSize, CV_8UC3);
    m_canvasSize = canvasSize;

    cvui::init(PROJECT_NAME);
    m_port.init_com(comPort);

    //m_lastUpdate = std::chrono::system_clock::now();
    snake_data = new mutex();
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
    
    SNAKE_DIRECTION newDirection = analogToSnake(*currentPosition, m_currentDirection);
    if(newDirection != getOpposite(m_currentDirection))
    {
        m_currentDirection = newDirection;
    }
        
    
    m_stateS1 = m_port.get_button(CH_SWITCH_S1);
    m_stateS2 = m_port.get_button(CH_SWITCH_S2);

    update_colour();
    changeLED();
}

void CSnakeGame::update_colour()
{
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
    }
}

void CSnakeGame::update()
{
    m_resetProgram += m_stateS1;

    snake_data->lock();
    if(m_resetProgram)
    {
        reset();
    }

    moveSnake();
    snake_data->unlock();
}

void CSnakeGame::draw()
{
    snake_data->lock();
    auto thisFrameStartTime = chrono::system_clock::now();
    auto frameTime = thisFrameStartTime - m_updateStartTime;

    m_fpsRate = 1000000.0/chrono::duration_cast<chrono::microseconds>(frameTime).count();
    m_sleepUntil = thisFrameStartTime + chrono::microseconds(1000000/TARGET_FRAME_RATE) - chrono::milliseconds(THREAD_SLEEP_OFFSET);

    m_updateStartTime = thisFrameStartTime;

    m_canvas = cv::Mat::zeros(m_canvasSize, CV_8UC3);

    
    // Render snake
    for(int segmentIndex = 0; segmentIndex < m_snakeSegments.size(); segmentIndex++)
    {
        cv::Point segment = m_snakeSegments[segmentIndex];
        cv::Rect pixel(SNAKE_CANVAS_DIVISOR * segment.x, SNAKE_CANVAS_DIVISOR * segment.y, SNAKE_CANVAS_DIVISOR, SNAKE_CANVAS_DIVISOR);
        m_canvas(pixel) = m_snakeColour;
    }
    

    //Render UI
    render_ui();
    cvui::imshow(PROJECT_NAME, m_canvas);

    if(cv::waitKey(1) == 'q') m_endProgram = true;
    
    this_thread::sleep_until(m_sleepUntil);

    while (std::chrono::system_clock::now() < m_sleepUntil) {
        // Optionally yield CPU
        std::this_thread::yield();
    }

    snake_data->unlock();
}

void CSnakeGame::run()
{
    thread gpio_t(&CSnakeGame::run_gpio, this);
    gpio_t.detach();

    thread update_t(&CSnakeGame::run_update, this);
    update_t.detach();

    do
    {
        draw();
    }
    while(!m_endProgram);
}

void CSnakeGame::run_gpio()
{
    while(!m_endProgram)
    {
        gpio();
    }
}

void CSnakeGame::run_update()
{
    while(!m_endProgram)
    {
        update();
    }
}

void CSnakeGame::reset()
{
    m_snakeColour = SCL_SNAKE_RED;
    m_currentSnakeColour = SNAKE_RED;

    //snake init
    m_snakeSegments.clear();
    for(int segment = 0; segment < 20; segment++)
    {
        m_snakeSegments.push_back(cv::Point((m_canvasSize.width / SNAKE_CANVAS_DIVISOR - 1) / 2 + segment,
            (m_canvasSize.height / SNAKE_CANVAS_DIVISOR - 1) / 2));
    }

    m_currentDirection = SNAKE_DIRECTION_STOP;
    m_resetProgram = false;
}

void CSnakeGame::render_ui()
{
    cvui::beginRow(m_canvas, 10,10, -1, -1, 5);
    stringstream windowLabel;
    windowLabel << PROJECT_NAME 
        << " (" << m_canvasSize.width << ", "
        << m_canvasSize.height << ") ("
        << setprecision(8) << m_fpsRate << "FPS)";
        cout << setprecision(8) << m_fpsRate << endl;
    cvui::window(200,80, windowLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 40, -1, -1, 5);
    stringstream colourLabel;
    colourLabel << "Colour : "  << getSnakeColourName();
    cvui::text(colourLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 60, -1, 10, 5);
    cvui::trackbar(m_canvas, 16, 60, 180, &m_selectedFPS, (float)10.0, (float)120.0);
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 120, -1, 10, 5);
    if(cvui::button("Reset"))
    {
        m_resetProgram = true;
    }

    if(cvui::button("Exit"))
    {
        m_endProgram = true;
    }
    cvui::endRow();
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
    m_port.set_data(TYPE_ANALOG, CH_RGBLED_RED_PIN, 0);
    m_port.set_data(TYPE_ANALOG, CH_RGBLED_GRN_PIN, 0);
    m_port.set_data(TYPE_ANALOG, CH_RGBLED_BLU_PIN, 0);
    switch(m_currentSnakeColour)
    {
        case SNAKE_RED:
            m_port.set_data(TYPE_ANALOG, CH_RGBLED_RED_PIN, LED_BRIGHTNESS);
            break;
        case SNAKE_GREEN:
            m_port.set_data(TYPE_ANALOG, CH_RGBLED_GRN_PIN, LED_BRIGHTNESS);
            break;
        case SNAKE_BLUE:
            m_port.set_data(TYPE_ANALOG, CH_RGBLED_BLU_PIN, LED_BRIGHTNESS);
            break;
    }
}

void CSnakeGame::moveSnake()
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

bool CSnakeGame::validateSnake(cv::Point_<int> newHead)
{
    for(int segment = 0; segment < m_snakeSegments.size() - 1; segment++)
    {
        if(newHead == m_snakeSegments[segment])
            return false;
    }
}

SNAKE_DIRECTION CSnakeGame::analogToSnake(CJoystickPosition input, SNAKE_DIRECTION currentDirection)
{
    JOYSTICK_DIRECTION joystickDirection = input.get_simple_direction();

    switch(joystickDirection)
    {
        case JOYSTICK_DIRECTION_NORTH:
            return SNAKE_DIRECTION_NORTH;
            break;
        case JOYSTICK_DIRECTION_EAST:
            return SNAKE_DIRECTION_EAST;
            break; 
        case JOYSTICK_DIRECTION_SOUTH: 
            return SNAKE_DIRECTION_SOUTH;
            break;
        case JOYSTICK_DIRECTION_WEST:
            return SNAKE_DIRECTION_WEST;
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