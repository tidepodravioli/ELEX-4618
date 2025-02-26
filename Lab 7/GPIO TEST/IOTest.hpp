/**
 * @file IOTest.hpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief 
 * @version 0.1
 * @date 2025-01-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "getstuff.h"
#include "CControlPi.hpp"

#define MAX_SERIAL_ERRORS 10
#define BUTTON_DEBOUNCE 1000
#define COLUMN_WIDTH 32

#define QUIT_KEY 'q'


using namespace std;
 /**
  * @brief 
  * 
  */
class IOTest
{
    private:
    CControlPi * m_port;

    void printMenu();
    char get_key();

    void joystickTest();
    void buttonTest();
    void servoTest();
    void digitalTest();

    //void delay(int ms);
    void init_kbhit();
    void end_kbhit();
    
    bool m_init = false;
    public:
    IOTest();
    ~IOTest();
    void run();
};