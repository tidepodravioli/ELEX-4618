#include <../Common/CBase4618.hpp>
#include <headers/CAsteroid.hpp>
#include <headers/CMissle.hpp>
#include <headers/CShip.hpp>
#include <../Common/CJoystickPosition.hpp>

#include <vector>
#include <thread>

#include <chrono>

#define BUTTON_DEBOUNCE_TIMEOUT 250 ///< Debounce for the buttons in gpio (in ms)

#define CASTEROIDGAME_PROGRAM_TITLE "Lab 6 - Asteroids"
#define CASTEROIDGAME_MAX_ASTEROIDS 20 ///< Maximum amount of asteroids allowed on the game field
#define CASTEROIDGAME_NEXT_ASTEROID 1 ///< Time to wait until the next asteroid is generated (in seconds)

using namespace cv;
using namespace std;

class CAsteroidGame : CBase4618
{
    private:
    CAsteroidGame();
    CJoystickPosition m_currentPosition;
    bool m_S1pressed = false;
    bool m_S2pressed = false;
    Size m_canvasSize;
    CShip * m_ship;
    vector<CMissle> m_missles;
    vector<CAsteroid> m_asteroids;

    chrono::system_clock::time_point m_timeNextAsteroid;

    bool m_flagEndProgram = false;
    bool m_flagGameOver = false;
    bool m_flagGameReset = false;
    bool m_flagGenerateAsteroid = true;
    
    int m_playerScore = 0;

    void drawUI();
    void updateShipAccel();
    void generateMissle();
    void updateMissle();
    void generateAsteroid();
    public:
    CAsteroidGame(Size canvasSize);

    void gpio();
    void update();
    void draw();
    void run();
    void reset();

    void thread_gpio();
    void thread_update();
};