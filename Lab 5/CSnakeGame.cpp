#include <CSnakeGame.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui\cvui.h"

CSnakeGame::CSnakeGame(cv::Size canvasSize, int comPort)
{
    m_canvas = cv::Mat::zeros(canvasSize, CV_8UC3);
    m_canvasSize = canvasSize;
    m_appleLocation = cv::Point(-1,-1);

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
        m_flagUpdateColour = true;
        
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
        this_thread::sleep_until(runUntil); //fixes the update rate
    }
    else
    {
        //cout << "Game over" << endl;   
    }
}

void CSnakeGame::draw()
{
    snake_data->lock();
    //frame rate measurement setup
    auto thisFrameStartTime = chrono::system_clock::now();
    auto frameTime = thisFrameStartTime - m_drawStartTime;

    m_fpsRate = 1000000.0/chrono::duration_cast<chrono::microseconds>(frameTime).count();
    m_drawSleepUntil = thisFrameStartTime + chrono::microseconds(1000000/TARGET_FRAME_RATE) - chrono::milliseconds(THREAD_SLEEP_OFFSET);
    m_drawStartTime = thisFrameStartTime;

    m_canvas = cv::Mat::zeros(m_canvasSize, CV_8UC3);

    //Render UI
    render_ui();

    // Render snake
    for(int segmentIndex = 0; segmentIndex < m_snakeSegments.size(); segmentIndex++)
    {
        cv::Point segment = m_snakeSegments[segmentIndex];
        cv::Rect pixel((int)m_stepSize * segment.x, (int)m_stepSize * segment.y, (int)m_stepSize, (int)m_stepSize);
        m_canvas(pixel) = m_snakeColour;
    }
    //Render apple
    if(m_flagApple)
    {
        const int appleX = (int)(m_appleLocation.x * (m_stepSize));
        const int appleY = (int)(m_appleLocation.y * (m_stepSize));
        const cv::Point appleLocation(appleX, appleY);
        //cv::circle(m_canvas, appleLocation, m_stepSize, m_snakeColour);
        cv::Rect apple(appleX, appleY, (int)m_stepSize, (int)m_stepSize);
        m_canvas(apple) = m_snakeColour;
    }
    snake_data->unlock();

    cvui::imshow(PROJECT_NAME, m_canvas);

    if(cv::waitKey(1) == 'q') m_flagEndProgram = true;

    //limit frame rate by sleeping until 1/30 of a second has passed since the start of the frame drawing
    this_thread::sleep_until(m_drawSleepUntil);
    while (std::chrono::system_clock::now() < m_drawSleepUntil) {
        // Optionally yield CPU
        std::this_thread::yield();
    }


}

void CSnakeGame::run()
{
    thread gpio_t(&CSnakeGame::run_gpio, this);
    gpio_t.detach();

    thread update_t(&CSnakeGame::run_update, this);
    update_t.detach();

    thread draw_t(&CSnakeGame::run_draw, this);
    draw_t.detach();

    while(!m_flagEndProgram){}
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

void CSnakeGame::run_draw()
{
    cvui::init(PROJECT_NAME);
    while(!m_flagEndProgram)
    {
        draw();
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
        const int midWidth = (m_canvasSize.width / (int)m_stepSize - 1) / 2;
        const int midHeight = (m_canvasSize.height / (int)m_stepSize - 1) / 2;

        const cv::Point newSegment(midWidth + segment, midHeight);
        m_snakeSegments.push_back(newSegment);
        print_point(newSegment);
    }

    m_currentDirection = SNAKE_DIRECTION_STOP;
    m_flagResetProgram = false;
    m_flagGameOver = false;
    m_snakeScore = 0;
}

void CSnakeGame::render_ui()
{
    cvui::beginRow(m_canvas, 10,10, -1, -1, 5);
    stringstream windowLabel;
    windowLabel << PROJECT_NAME 
        << " (" << m_canvasSize.width << ", "
        << m_canvasSize.height << ") ("
        << setprecision(8) << m_fpsRate << "FPS)";
    cvui::window(200, 200, windowLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 40, -1, -1, 5);
    stringstream colourLabel;
    colourLabel << "Colour : "  << getSnakeColourName();
    cvui::text(colourLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 60, -1, -1, 5);
    stringstream scoreLabel;
    scoreLabel << "Score : " << m_snakeScore;
    cvui::text(scoreLabel.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 75, 80, -1, 10, 5);
    cvui::text("Step Size");
    bool sizeUpdated = cvui::trackbar(m_canvas, 16, 80, 180, &m_stepSize, (float)1.05, (float)20.0);
    if(sizeUpdated)
    {
        extrapolateSnake(m_lastStepSize, (int)m_stepSize);
        m_lastStepSize = (int)m_stepSize;
    }
    cvui::endRow();

    cvui::beginRow(m_canvas, 60, 130, -1, 10, 5);
    cvui::text("Snake Speed");
    cvui::trackbar(m_canvas, 16, 130, 180, &m_updatePeriod, (float)10, (float)500);
    cvui::endRow();

    cvui::beginRow(m_canvas, 15, 180, -1, 10, 5);
    if(cvui::button("Reset"))
    {
        m_flagResetProgram = true;
    }

    if(cvui::button("Exit"))
    {
        m_flagEndProgram = true;
    }
    cvui::endRow();

    if(m_flagGameOver)
    {
        const int centerX = m_canvasSize.width / 2;
        const int centerY = m_canvasSize.height / 2;

        cvui::text(m_canvas, centerX, centerY, "GAME OVER", 0.5);
    }
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
    if(m_flagUpdateColour)
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

        m_flagUpdateColour = false;
    }
    
}

void CSnakeGame::updateSnake(bool addToSnake)
{
    cv::Point head = m_snakeSegments.back();
    cv::Point newHead = cv::Point(0,0);
    
    switch(m_currentDirection)
    {
        case SNAKE_DIRECTION_NORTH:
            newHead = cv::Point(head.x, head.y - 1);
            break;
        case SNAKE_DIRECTION_EAST:
            newHead = cv::Point(head.x + 1, head.y);
            break;
        case SNAKE_DIRECTION_SOUTH:
            newHead = cv::Point(head.x, head.y + 1);
            break;
        case SNAKE_DIRECTION_WEST:
            newHead = cv::Point(head.x - 1, head.y);
            break;
        default:
            newHead = head;
            break;

    }
    
    if(head != newHead)
    {
        SNAKE_STATUS snakeStatus = validateSnake(newHead);

        if(snakeStatus != SNAKE_DEAD)
        {
            m_snakeSegments.push_back(newHead);
            if(!addToSnake)
            {
                m_snakeSegments.erase(m_snakeSegments.begin());
            }
            m_flagAddToSnake = false;

            if(snakeStatus == SNAKE_APPLE)
            {
                cout << "APPLE" << endl;
                m_flagApple = false;
                m_flagAddToSnake = true;
                m_snakeScore++;
                m_nextApple = chrono::system_clock::now() + chrono::seconds(5);
            }
        }
        else
        {
            m_flagGameOver = true;
        }
    }
}

SNAKE_STATUS CSnakeGame::validateSnake(cv::Point_<int> newHead)
{
    for(int segment = 0; segment < m_snakeSegments.size() - 1; segment++)
    {
        
        if(newHead == m_snakeSegments[segment])
        {
            cout << "DEAD : Snake ate tail" << endl;
            return SNAKE_DEAD;
        }

    }

    if(newHead == m_appleLocation && m_flagApple)
        return SNAKE_APPLE;

    const int x_gridLimit = (m_canvasSize.width / (int)m_stepSize) - 1;
    const int y_gridLimit = (m_canvasSize.height / (int)m_stepSize) - 1;

    const int headX = newHead.x;
    const int headY = newHead.y;


    if(0 > headX ||  headX > x_gridLimit)
    {
        return SNAKE_DEAD;
        cout << "DEAD : Snake out of bounds (X)" << endl;
    }

    else if(0 > headY || headY > y_gridLimit)
    {
        return SNAKE_DEAD;
        cout << "DEAD : Snake out of bounds (Y)" << endl;
    }

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

void CSnakeGame::extrapolateSnake(int originalStep, int newStep)
{
    cout << "Extrapolation" << endl;
    const int canvasX = m_canvasSize.width;
    const int canvasY = m_canvasSize.height;

    for(int segment = 0; segment < m_snakeSegments.size(); segment++)
    {
        const int segX = m_snakeSegments[segment].x;
        const int segY = m_snakeSegments[segment].y;

        const float o_percentX = (float)segX / (canvasX / originalStep);
        const float o_percentY = (float)segY / (canvasY / originalStep);

        const int new_segX = (int)(o_percentX * (canvasX / newStep));
        const int new_segY = (int)(o_percentY * (canvasY / newStep));

        const cv::Point_<int> extrapolatedSegment(new_segX, new_segY);

        m_snakeSegments[segment] = extrapolatedSegment;
        print_point(extrapolatedSegment);
    }

    if(m_flagApple)
    {
        const int appleX = m_appleLocation.x;
        const int appleY = m_appleLocation.y;

        const float o_percentX = (float)appleX / (canvasX / originalStep);
        const float o_percentY = (float)appleY / (canvasY / originalStep);

        const int new_segX = (int)(o_percentX * (canvasX / newStep));
        const int new_segY = (int)(o_percentY * (canvasY / newStep));

        const cv::Point_<int> extrapolatedSegment(new_segX, new_segY);

        m_appleLocation = extrapolatedSegment;
    }
}

void CSnakeGame::print_point(cv::Point_<int> point)
{
    stringstream formatbuilder;
    formatbuilder << "("
        << point.x << ", "
        << point.y << ")"
        << endl;

    cout << formatbuilder.str();
}