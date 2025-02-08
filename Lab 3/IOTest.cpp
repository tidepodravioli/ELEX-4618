#include "IOTest.hpp"

using namespace std;
using namespace raf_cin;

IOTest::IOTest()
{
    m_port.init_com(6);
}

void IOTest::run()
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
            case 'c':
                connectionSetup();
                break;
            default:
                cout << "Invalid command. See menu." << endl;
                break;
        }
    }
    while(keyPressed != 'q');
    cout << "Exiting..." << endl;
}

void IOTest::joystickTest()
{
    cout << "Starting joystick test..." << endl;

    int errorCount = 0;
    while(!_kbhit())
    {   
        autoConnect();
        bool pass = false;
        CJoystickPosition position = m_port.get_analog(pass);
        
        if(pass)
        {   
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
        }
        else
        {
            if(errorCount < MAX_SERIAL_ERRORS)
            {
                cout << "Serial communication or value received error. Trying again." << endl;
                errorCount++;
                continue;
            }
            else
            {
                cout << "Exiting since the serial port is unavailable." << endl;
                break;
            }
        } 
    }

    cout << "Detected keypress, exiting to main menu..." << endl;
}

void IOTest::buttonTest()
{
    cout << "Starting button test..." << endl;
    
    int clickCount = 0;
    while(!_kbhit())
    {
        bool result = m_port.get_button(0);
        int errorCount = 0;

        if(result)
        {
            clickCount++;
            cout << "BUTTON TEST: " << clickCount << endl;
        }
        
    }
    cout << "Detected keypress, exiting to main menu..." << endl;
}

void IOTest::servoTest()
{
    while(!_kbhit())
    {
        autoConnect();
        bool pass = m_port.set_data(TYPE_SERVO, 0, 180);
        while(!pass)
        {
            pass = m_port.set_data(TYPE_SERVO, 0, 180);
        }
        cout << "SERVO TEST: CH-S0 : 180" << endl;
        delay(BUTTON_DEBOUNCE_TIMEOUT);

        m_port.set_data(TYPE_SERVO, 0, -180);
        while(!pass)
        {
            pass = m_port.set_data(TYPE_SERVO, 0, -180);
        }
        cout << "SERVO TEST: CH-S0 : -180" << endl;
        delay(BUTTON_DEBOUNCE_TIMEOUT);
    }
}

void IOTest::digitalTest()
{
    while(!_kbhit())
    {
        autoConnect();
        int buttonState = -1;
        bool getPass = m_port.get_data(TYPE_DIGITAL, 1, buttonState);

        if(getPass)
        {
            bool setPass = m_port.set_data(TYPE_DIGITAL, 3, buttonState);
            while(!setPass)
            {
                setPass = m_port.set_data(TYPE_DIGITAL, 2, buttonState);
            }
            cout << "DIGITAL TEST: CH-D2 : " << buttonState << endl;
        }
    }
}

void IOTest::printMenu()
{
    cout << "*******************************************" << endl;
    cout << "ELEX4618 Lab 3 IO Test, by Rafael Banalan" << endl;
    cout << "*******************************************" << endl;
    cout << "Press any key during a test to exit back to this menu" << endl;
    cout << "(J)oystick test" << endl;
    cout << "(D)igital test" << endl;
    cout << "(B)utton test" << endl;
    cout << "(S)ervo write" << endl;
    cout << "(C)onnection settings" << endl;
    cout << "(Q)uit" << endl;
    cout << "CMD> " << std::flush;
}

void IOTest::connectionSetup()
{
    cout << "Enter number of COM port to connect to: ";
    
    int port = 0;
    while(!get_int(&port))
    {
        cout << "Invalid port number, try again: ";
    }
    
    cout << endl << "Connecting to COM" << port << endl;
    m_port.init_com(port);
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
    auto startTime = std::chrono::steady_clock::now();
    while(true)
    {
        // * ChatGPT code that has been modified
        auto elapsed_time = std::chrono::steady_clock::now() - startTime;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() > ms) {
            break;
        }
    }
}

void IOTest::autoConnect()
{
    while(!m_port.checkPort())
    {
        m_port.findPort();
    }

    return;
}

