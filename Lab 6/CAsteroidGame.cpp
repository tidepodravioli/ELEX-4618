#include <headers/CAsteroidGame.hpp>

#define CVUI_IMPLEMENTATION
#include <../Common/cvui/cvui.h>

CAsteroidGame::CAsteroidGame(Size canvasSize)
{
    m_canvasSize = canvasSize;
    cvui::init(CASTEROIDGAME_PROGRAM_TITLE);

    m_ship = new CShip(Point2i(canvasSize.width/2,canvasSize.height/2));
    m_port.init_com(9);
}

void CAsteroidGame::gpio()
{   
    m_timeLastGPIO = chrono::system_clock::now();
    if(!m_port.checkPort())
    {
        m_flagSerialConnected = false;
        setupGPIO();
    }
    
    bool joystickIOpass = false;
    int failCount = 0;
    do
    {
        if(failCount > CASTEROIDGAME_MAX_SERIAL_ERRORS) break;

        m_currentPosition = m_port.get_analog(joystickIOpass);
        if(!joystickIOpass) failCount++;
        else break;
    } 
    while (true);
    
    if(!joystickIOpass)
    {
        m_flagSerialConnected = false;
        setupGPIO();
        return;
    }
    else m_flagSerialConnected = true;
    
    m_S1pressed = m_port.get_button(CH_SWITCH_S1);
    m_S2pressed = m_port.get_button(CH_SWITCH_S2);
}

void CAsteroidGame::thread_gpio()
{
    while(!m_flagEndProgram)
    {
        gpio();
    }
}

void CAsteroidGame::setupGPIO()
{
    if(!m_port.checkPort())
    {
        cout << "Serial port disconnected... trying again." << endl;
        do
        {
            m_port.findPort();
        } while(!m_port.checkPort());
    }

    m_flagSerialConnected = true;
}

void CAsteroidGame::update()
{
    // the time the update cycle is supposed to finish at
    auto sleepTil = chrono::system_clock::now() + chrono::milliseconds(10);
    
    if(chrono::system_clock::now() - m_timeLastGPIO > chrono::seconds(CASTEROIDGAME_SERIAL_TIME_MAX))
    {
        m_flagSerialConnected = false;
    }
    if(m_S2pressed)
    {
        cout << "EVENT : Reset button pressed." << endl;
        reset();
        m_S2pressed = false;
    }

    if(!m_flagGameOver)
    {
        updateShipAccel();

        if(m_S1pressed)
        {
            cout << "EVENT : Fire button pressed." << endl;
            generateMissle();
            m_S1pressed = false;
        }
    
        if(m_flagGameReset) reset();

        if(m_ship->get_lives() <= 0) m_flagGameOver = true;
    
        updateMissle();
    
        if(m_flagGenerateAsteroid)
        {
            generateAsteroid();
            m_timeNextAsteroid = chrono::system_clock::now() + chrono::milliseconds(CASTEROIDGAME_NEXT_ASTEROID);

        }
    
        if(m_asteroids.size() < CASTEROIDGAME_MAX_ASTEROIDS && chrono::system_clock::now() > m_timeNextAsteroid)
        {
            m_flagGenerateAsteroid = true;
        }
    
        for(int asteroid = 0; asteroid < m_asteroids.size(); asteroid++)
        {
            m_asteroids[asteroid].move();
    
            //check if any of the missles has hit the asteroid we're currently looking at
            for(int missle = 0; missle < m_missles.size(); missle++)
            {
                if(m_asteroids[asteroid].collide(m_missles[missle]))
                {
                    m_asteroids[asteroid].hit();
                    m_missles[missle].hit();
                    m_playerScore += 10;
                }
            }
    
            // if the asteroid has no more lives, delete it
            if(m_asteroids[asteroid].get_lives() <= 0)
            {
                m_asteroids.erase(m_asteroids.begin() + asteroid);
                asteroid--;
                continue;
            }
    
            // if the asteroid has hit a wall, make it bounce
            if(m_asteroids[asteroid].collide_wall(m_canvasSize))
            {
                m_asteroids[asteroid].bounce(m_canvasSize);
            }
    
            // if the ship has collided with the asteroid, call both of their hit functions
            if(m_ship->collide(m_asteroids[asteroid]))
            {
                if(m_flagSerialConnected) m_ship->hit();
                m_asteroids[asteroid].hit();
            }
        }
    }

    this_thread::sleep_until(sleepTil);
}

void CAsteroidGame::thread_update()
{
    while(!m_flagEndProgram)
    {
        update();
    }
}


void CAsteroidGame::draw()
{
    m_canvas = Mat::zeros(m_canvasSize, CV_8UC3);
    m_ship->draw(m_canvas);

    for(int missle = 0; missle < m_missles.size(); missle++)
    {
        m_missles[missle].draw(m_canvas);
    }

    for(int asteroid = 0; asteroid < m_asteroids.size(); asteroid++)
    {
        m_asteroids[asteroid].draw(m_canvas);
    }

    drawUI();

    cvui::imshow(CASTEROIDGAME_PROGRAM_TITLE, m_canvas);
    m_flagEndProgram = waitKey(1) == 'q';
}

void CAsteroidGame::run()
{
    thread gpio_t(&CAsteroidGame::thread_gpio, this);
    gpio_t.detach();

    thread update_t(&CAsteroidGame::thread_update, this);
    update_t.detach();

    while(!m_flagEndProgram)
    {
        draw();
    }
}

void CAsteroidGame::reset()
{
    m_missles.clear();
    m_asteroids.clear();
    m_ship = new CShip(Point2i(m_canvasSize.width/2,m_canvasSize.height/2));

    m_playerScore = 0;
    m_flagGameOver = false;
    m_flagGameReset = false;
}

void CAsteroidGame::updateShipAccel()
{
    // sets the acceleration based on the joystick position
    const int pX = m_currentPosition.percentX();
    const int pY = m_currentPosition.percentY();

    const float accX = (3.0/100.0) * (float)m_currentPosition.percentX() - 1.5;
    const float accY = (-3.0/100.0) * ((float)m_currentPosition.percentY() - 100) - 1.5;
    
    if(m_currentPosition.joystickActive())
        m_ship->set_acceleration(Point2f(accX,accY));
    else
        m_ship->set_acceleration(Point2f(0,0));

    // update the ships location
    m_ship->move(m_canvasSize);
    //cout << m_currentPosition.getX() << ", " << m_currentPosition.getY() << endl;
}

void CAsteroidGame::drawUI()
{
    cvui::beginRow(m_canvas, 10, 10);
    stringstream livesCount;
    livesCount << "Lives : " << m_ship->get_lives();
    cvui::text(livesCount.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 10, 20);
    stringstream missleCount;
    missleCount << "Missles : " << m_missles.size();
    cvui::text(missleCount.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 10, 30);
    stringstream asteroidCount;
    asteroidCount << "Asteroids : " << m_asteroids.size();
    cvui::text(asteroidCount.str());
    cvui::endRow();

    cvui::beginRow(m_canvas, 10, 40);
    stringstream scoreCount;
    scoreCount << "Score : " << m_playerScore;
    cvui::text(scoreCount.str());
    cvui::endRow();

    if(m_flagGameOver)
    {
        drawCenteredText(m_canvas, "GAME OVER");
    }
    
    if(!m_flagSerialConnected)
    {
        drawCenteredText(m_canvas, "SERIAL DISCONNECTED");
    }
}

void CAsteroidGame::generateMissle()
{
    CMissle missle;
    missle.set_pos(m_ship->get_pos_head());
    m_missles.push_back(missle);
}

void CAsteroidGame::updateMissle()
{
    for(int missle = 0; missle < m_missles.size(); missle++)
    {
        if((m_missles[missle].collide_wall(m_canvasSize)) || (m_missles[missle].get_lives() <= 0))
        {
            m_missles.erase(m_missles.begin() + missle);
            missle--;
            continue;
        }

        m_missles[missle].move();
    }
}

void CAsteroidGame::generateAsteroid()
{
    CAsteroid asteroid(m_canvasSize);
    m_asteroids.push_back(asteroid);
    m_flagGenerateAsteroid = false;
    m_timeNextAsteroid = chrono::system_clock::now() + chrono::seconds(CASTEROIDGAME_NEXT_ASTEROID);
}

// chatgpt generated code
void CAsteroidGame::drawCenteredText(cv::Mat &img, const std::string &text, int fontFace,
    double fontScale, cv::Scalar color, int thickness) {
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    baseline += thickness;

    // Calculate the position to center the text
    cv::Point textOrg((img.cols - textSize.width) / 2, (img.rows + textSize.height) / 2);

    // Draw the text
    cv::putText(img, text, textOrg, fontFace, fontScale, color, thickness, cv::LINE_AA);
}
