#include "Lab9Server.hpp"

using namespace std;

Lab9Server::Lab9Server()
{
    thread service(&Lab9Server::server_start, this);
    service.detach();

    cout << "SERVER : Started server" << endl;
    run();
}

void Lab9Server::run()
{
    while(!m_flagCloseServer)
    {
        vector<string> cmds;
        m_server.get_cmd(cmds);
        
        if(!cmds.empty())
        {
            cout << endl << "Detected input : "; 
            cout << cmds[0];
            cout << endl;

            if(cmds[0].compare("S 0 1\n") >= 0)
            {
                m_server.send_string("A 0 1\n");
                cout << "SERVER : System ON command acknowledged" << endl;
            }
            else if(cmds[0].compare("S 0 0\n") >= 0)
            {
                m_server.send_string("A 0 1\n");
                cout << "SERVER : System OFF command acknowledged" << endl;
            }
        }
    }
}

void Lab9Server::server_start()
{
    m_server.start(LAB9SERVER_PORT);
}