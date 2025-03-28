#include "Lab9Server.hpp"

using namespace std;

Lab9Server::Lab9Server()
{
    thread service(&Lab9Server::server_start, this);
    service.detach();

    cout << "SERVER : Started server" << endl;
}

void 

void Lab9Server::server_start()
{
    m_server.start(LAB9SERVER_PORT);
}