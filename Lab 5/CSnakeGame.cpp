#include <CSnakeGame.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui\cvui.h"

CSnakeGame::CSnakeGame(cv::Size canvasSize, int comPort)
{
    m_canvas = cv::Mat::zeros(canvasSize, CV_8UC3);
    m_canvasSize = canvasSize;
    m_appleLocation = cv::Point(-1,-1);

    cvui::init(PROJECT_NAME);
    m_port.init_com(comPort);

    snake_data = new mutex();
    srand(time(NULL));
    reset();
}

void CSnakeGame::gpio()
{
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
    m_flagResetProgram += m_stateS1;
    if(m_flagResetProgram) reset();

    if(!m_flagGameOver)
    {
        auto runUntil = chrono::system_clock::now() + chrono::milliseconds((int)m_updatePeriod);

        updateSnake(m_flagAddToSnake);
        createApple();
        SNAKE_STATUS snakeStatus = validateSnake(m_snakeSegments.back());
        if(snakeStatus == SNAKE_DEAD) m_flagGameOver = true;
        else if(snakeStatus == SNAKE_APPLE)
        {
            cout << "APPLE" << endl;
            m_flagApple = false;
            m_flagAddToSnake = true;
            m_nextApple = chrono::system_clock::now() + chrono::seconds(5);
        }

        this_thread::sleep_until(runUntil);
    }
    else
    {
        cout << "Game over" << endl;   
    }
}

void CSnakeGame::draw()
{
    m_canvas = cv::Mat::zeros(m_canvasSize, CV_8UC3);

    
    // Render snake
    for(int segmentIndex = 0; segmentIndex < m_snakeSegments.size(); segmentIndex++)
    {
        cv::Point segment = m_snakeSegments[segmentIndex];
        cv::Rect pixel((int)m_stepSize * segment.x, (int)m_stepSize * segment.y, (int)m_stepSize, (int)m_stepSize);
        m_canvas(pixel) = m_snakeColour;
    }
    
    if(m_flagApple)
    {
        const int appleX = (int)(m_appleLocation.x * (m_stepSize));
        const int appleY = (int)(m_appleLocation.y * (m_stepSize));
        const cv::Point appleLocation(appleX, appleY);
        //cv::circle(m_canvas, appleLocation, m_stepSize, m_snakeColour);
        cv::Rect apple(appleX, appleY, (int)m_stepSize, (int)m_stepSize);
        m_canvas(apple) = m_snakeColour;
    }

    //Render UI
    render_ui();
    cvui::imshow(PROJECT_NAME, m_canvas);

    if(cv::waitKey(1) == 'q') m_flagEndProgram = true;
}

void CSnakeGame::run()
{
    thread gpio_t(&CSnakeGame::run_gpio, this);
    gpio_t.detach();

    thread update_t(&CSnakeGame::run_update, this);
    update_t.detach();

    do
    {
        //frame rate measurement setup
        auto thisFrameStartTime = chrono::system_clock::now();
        auto frameTime = thisFrameStartTime - m_drawStartTime;

        m_fpsRate = 1000000.0/chrono::duration_cast<chrono::microseconds>(frameTime).count();
        m_drawSleepUntil = thisFrameStartTime + chrono::microseconds(1000000/TARGET_FRAME_RATE) - chrono::milliseconds(THREAD_SLEEP_OFFSET);
        m_drawStartTime = thisFrameStartTime;

        draw();

        //limit frame rate by sleeping until 1/30 of a second has passed since the start of the frame drawing
        this_thread::sleep_until(m_drawSleepUntil);
        while (std::chrono::system_clock::now() < m_drawSleepUntil) {
            // Optionally yield CPU
            std::this_thread::yield();
        }
    }
    while(!m_flagEndProgram);
}

void CSnakeGame::run_gpio()
{
    while(!m_flagEndProgram)
    {
        gpio();
    }
}

void CSnakeGame::run_update()
{
    while(!m_flagEndProgram)
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
    m_flagResetProgram = false;
    m_flagGameOver = false;
}

void CSnakeGame::render_ui()
{
    cvui::beginRow(m_canvas, 10,10, -1, -1, 5);
    stringstream windowLabel;
    windowLabel << PROJECT_NAME 
        << " (" << m_canvasSize.width << ", "
        << m_canvasSize.height << ") ("
        << setprecision(8) << m_fpsRate << "FPS)";
    //cout << setprecision(8) << m_fpsRate << endl;
    cvui::window(200,80, windowLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 40, -1, -1, 5);
    stringstream colourLabel;
    colourLabel << "Colour : "  << getSnakeColourName();
    cvui::text(colourLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 60, -1, 10, 5);
    cvui::trackbar(m_canvas, 16, 60, 180, &m_stepSize, (float)1.0, (float)20.0);
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 90, -1, 10, 5);
    cvui::trackbar(m_canvas, 16, 90, 180, &m_updatePeriod, (float)10, (float)500);
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 140, -1, 10, 5);
    if(cvui::button("Reset"))
    {
        m_flagResetProgram = true;
    }

    if(cvui::button("Exit"))
    {
        m_flagEndProgram = true;
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

void CSnakeGame::updateSnake(bool addToSnake)
{
    cv::Point head = m_snakeSegments.back();
    cv::Point newHead = cv::Point(0,0);
    bool valid = false;
    if(m_currentDirection == SNAKE_DIRECTION_NORTH)
    {
        if(head.y > 0)
        {
            newHead = cv::Point(head.x, head.y - 1);
            valid = true;
        }
    }
    else if(m_currentDirection == SNAKE_DIRECTION_EAST)
    {
        if(head.x < (m_canvasSize.width / (int)m_stepSize) - 1)
        {
            newHead = cv::Point(head.x + 1, head.y);
            valid = true;
        }
    }
    else if(m_currentDirection == SNAKE_DIRECTION_SOUTH)
    {
        if(head.y < (m_canvasSize.height / (int)m_stepSize) - 1)
        {
            newHead = cv::Point(head.x, head.y + 1);
            valid = true;
        }
    }
    else if(m_currentDirection == SNAKE_DIRECTION_WEST)
    {
        if(head.x > 0)
        {
            newHead = cv::Point(head.x - 1, head.y);
            valid = true;
        }
    }

    if(valid)
    {
        m_snakeSegments.push_back(newHead);
        if(!addToSnake)
        {
            m_snakeSegments.erase(m_snakeSegments.begin());
        }
        m_flagAddToSnake = false;
    }
}

SNAKE_STATUS CSnakeGame::validateSnake(cv::Point_<int> newHead)
{
    for(int segment = 0; segment < m_snakeSegments.size() - 1; segment++)
    {
        if(newHead == m_snakeSegments[segment])
            return SNAKE_DEAD;
    }

    if(newHead == m_appleLocation && m_flagApple)
        return SNAKE_APPLE;

    return SNAKE_ALIVE;
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

void CSnakeGame::createApple()
{
    if(!m_flagApple)
    {
        if(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - m_nextApple).count() > 0)
        {
            const int x = rand() % (m_canvasSize.width / (int)m_stepSize) + 1;
            const int y = rand() % (m_canvasSize.height / (int)m_stepSize) + 1;

            m_appleLocation = cv::Point(x, y);
            m_flagApple = true;
        }
    }
    
}
