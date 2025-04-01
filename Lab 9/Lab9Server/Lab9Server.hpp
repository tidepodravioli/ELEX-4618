#include <thread>
#include <mutex>
#include <vector>

#define PI4618
#include "../ELEX4618-Template/server.h"

#define LAB9SERVER_PORT 4618

class Lab9Server
{
    private:
        CServer m_server;
        bool m_flagCloseServer = false;
        void server_start(); 

    public:
        Lab9Server();
        void run();
};