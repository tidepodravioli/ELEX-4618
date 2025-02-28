#include "IOTest.hpp"

using namespace std;
using namespace raf_cin;

IOTest::IOTest()
{
    m_port = new CControlPi(m_init);
}

IOTest::~IOTest()
{
    endwin();
}

void IOTest::init_kbhit()
{
    //ncurses init (from ChatGPT)
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
}

void IOTest::end_kbhit()
{
    endwin();
}

void IOTest::run()
{   
    if(m_init)
    {
        char keyPressed;
        do
        {
            printMenu();
            keyPressed = get_key();

            switch(keyPressed)
            {
                case 'j':
                    joystickTest();
                    break;
                case 'd':
                    digitalTest();
                    break;   
                case 'b':
                    buttonTest();
                    break;
                case 's':
                    servoTest();
                    break;
                default:
                    cout << "Invalid command. See menu." << endl;
                    break;
            }
        }
        while(keyPressed != 'q');
        cout << "Exiting..." << endl;
    }
    else
    {
        cout << "CControlPi was not able to be initialized..." << endl;
    }
}

void IOTest::joystickTest()
{
    cout << "Starting joystick test..." << endl;

    int errorCount = 0;
    while(true)
    {   
        bool pass = false;
        CJoystickPosition position = m_port->get_analog();
          
        stringstream column1;
        column1 << "ANALOG TEST: " << "CH-A0 : " << position.getX()
        << " (" << position.percentX()
        << "%)";

        stringstream column2;
        column2 << "CH-A1 : " << position.getY()
        << " (" << position.percentY()
        << "%)";

        cout << left  << setw(COLUMN_WIDTH) << column1.str() << setw(COLUMN_WIDTH) << column2.str() << endl; 
        errorCount = 0;

        init_kbhit();
        int ch = getch();
        if(ch == QUIT_KEY)
        {
            refresh();
            end_kbhit();
            break;
        }
        end_kbhit();
    }

    cout << "Detected keypress, exiting to main menu..." << endl;
}

void IOTest::buttonTest()
{
    cout << "Starting button test..." << endl;
    

    int clickCount = 0;
    while(true)
    {
        bool result = m_port->get_button(0);

        if(result)
        {
            clickCount++;
            cout << "BUTTON TEST: " << clickCount << endl;
        }

        init_kbhit();
        int ch = getch();
        if(ch == QUIT_KEY)
        {
            refresh();
            end_kbhit();
            break;
        }
        end_kbhit();
        
    }
    cout << "Detected keypress, exiting to main menu..." << endl;
}

void IOTest::servoTest()
{
    while(true)
    {
        m_port->set_servo(0, 180);

        cout << "SERVO TEST: CH-S0 : 180" << endl;
        delay(1000);

        m_port->set_servo(0, 0);

        cout << "SERVO TEST: CH-S0 : -180" << endl;
        delay(1000);

        init_kbhit();
        int ch = getch();
        if(ch == QUIT_KEY)
        {
            refresh();
            end_kbhit();
            break;
        }
        end_kbhit();

    }

    cout << "Detected keypress, exiting to main menu..." << endl;

}

void IOTest::digitalTest()
{
    while(true)
    {
        int buttonState = -1;
        m_port->get_data(0, buttonState);

        m_port->set_data(0, buttonState);
        cout << "DIGITAL TEST: CH-D2 : " << buttonState << endl;    
        
        init_kbhit();
        int ch = getch();
        if(ch == QUIT_KEY)
        {
            refresh();
            end_kbhit();
            break;
        }
        end_kbhit();
    }

    cout << "Detected keypress, exiting to main menu..." << endl;

}

void IOTest::printMenu()
{
    cout << "*******************************************" << endl;
    cout << "ELEX4618 Lab 3 IO Test, by Rafael Banalan" << endl;
    cout << "*******************************************" << endl;
    cout << "Press the q key during a test to exit back to this menu" << endl;
    cout << "(J)oystick test" << endl;
    cout << "(D)igital test" << endl;
    cout << "(B)utton test" << endl;
    cout << "(S)ervo write" << endl;
    cout << "(Q)uit" << endl;
    cout << "CMD> " << std::flush;
}


char IOTest::get_key()
{
    char keyPressed;
    get_char(&keyPressed);

    //if keyPressed is an uppercase letter, make it lowercase for the switch conditions
    if(keyPressed >= 'A' && keyPressed <= 'Z')
        keyPressed += TO_LOWERCASE;

    return keyPressed;
}

void IOTest::delay(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
