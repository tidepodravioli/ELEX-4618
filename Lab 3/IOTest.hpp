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
#include <conio.h>

#include "getstuff.h"
#include "CControl.h"

#define MAX_SERIAL_ERRORS 10
#define BUTTON_DEBOUNCE 1000
#define COLUMN_WIDTH 32


using namespace std;
 /**
  * @brief 
  * 
  */
class IOTest
{
    private:
    CControl m_port;
    void printMenu();
    char get_key();

    void joystickTest();
    void buttonTest();
    void servoTest();
    void digitalTest();

    void autoConnect();

    void connectionSetup();

    void delay(int ms);
    public:
    IOTest();
    void run();
};