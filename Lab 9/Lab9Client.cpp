#include "Lab9Client.hpp"

Lab9Client::Lab9Client()
{
    setup();
    run();
}

void Lab9Client::run()
{
    char keyPressed;
    do
    {
        menu();
        keyPressed = get_key();

        switch(keyPressed)
        {
            case '1':
                m_socket.tx_str("S 0 1\n");
                cout << "SET : Turn system ON" << endl;

                if(get_resp("A 0 1\n"))
                cout << "ACK : System ON" << endl;

            break;

            case '2':
                m_socket.tx_str("S 0 0\n");
                cout << "SET : Turn system OFF" << endl;
            break;

            case '3':

            break;

            case '4':

            break;
        }
    } while (keyPressed != 'q');
    
}

char Lab9Client::get_key()
{
    char keyPressed;
    raf_cin::get_char(&keyPressed);

    //if keyPressed is an uppercase letter, make it lowercase for the switch conditions
    if(keyPressed >= 'A' && keyPressed <= 'Z')
        keyPressed += TO_LOWERCASE;

    return keyPressed;
}

void Lab9Client::menu()
{
    cout << "*******************************************" << '\n';
    cout << "ELEX4618 Lab 9, by Rafael Banalan" << '\n';
    cout << "*******************************************" << '\n';
    cout << "(1) Turn system ON" << '\n';
    cout << "(2) Turn system OFF" << '\n';
    cout << "(3) Sort BIN1" << '\n';
    cout << "(4) Sort BIN2" << '\n';
    cout << "(Q)uit" << '\n';
    cout << "CMD> " << flush;
}

void Lab9Client::setup()
{
    cout    << "=================" << endl
            << "      SETUP" << endl
            << "=================" << endl;

    gui_openConnection();
}

void Lab9Client::gui_openConnection()
{
    m_socket = CClient();

    cout << "Please enter IP address of server : ";
        
    string IPaddr = "";
    while(!raf_cin::get_data(IPaddr, regex(E4618_IPADDR_REGEX)))
    {
        cout << "Please enter a valid IP address : ";
    }

    cout << "Please enter port for server : ";

    int port = 0;
    bool valid = false;
    while(!valid)
    {
        valid = !raf_cin::get_int(port);
        valid = (port >= 0 && port <= 65535);

        if(!valid) cout << "Please enter a valid port number : ";
    }

    cout << "Opening connection..." << endl;
    m_socket.connect_socket(IPaddr, port);
    
    cout << "Checking connection..." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    m_socket.tx_str(E4618_CHECKALIVE);
    
    string response = "";
    m_socket.rx_str(response);
    if(response.compare(E4618_ALIVEACK) >= 0)
    {
        cout << response;
        cout << "Connection established!" << endl;
    }
    else cout << "Connection not established" << endl;
}

bool Lab9Client::get_resp(const string f)
{
    string str = "";

    m_socket.rx_str(str);
    if(f.compare(str) >= 0) return true;
    else return false;
}