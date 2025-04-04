/**
 * @file Lab9Client.hpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Client program for Recycling Sorter 
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#define WIN4618
#include "ELEX4618-Template/Client.h"

#include "../Common/getstuff.h"

#define E4618_IPADDR_REGEX "^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)(\\.(?!$)|$)){4}$"
#define E4618_CHECKALIVE "G -1 -1\n"
#define E4618_ALIVEACK "A -1 -1\n"

using namespace std;

 class Lab9Client
 {
    private:
        void gui_openConnection();
        CClient m_socket;

        void menu();
        char get_key();
        bool get_resp(const string f);
    public:
        Lab9Client();
        void run();
        void setup();
 };