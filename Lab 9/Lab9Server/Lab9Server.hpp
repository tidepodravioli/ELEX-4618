#include <thread>
#include <mutex>

#define PI_4618
#include "../ELEX4618-Template/server.h"

#define LAB9SERVER_PORT 4618

class Lab9Server
{
    private:
        CServer m_server;
        void server_start(); 
    public:
        Lab9Server();
};