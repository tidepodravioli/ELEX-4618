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

#include "../Common/getstuff.h"

using namespace std;

 class Lab9Client
 {
    private:
        void gui_openConnection();
    public:
        Lab9Client();
        void run();
 };