/**
 * @file CGroceryManager.h
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Represents an instance of the grocery management program.
 * @version 0.1
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include "CGrocery.h"
#include "getstuff.h"

#define TO_LOWERCASE 32 //number to add to uppercase char to make it lowercase

using namespace std;

class CGroceryManager
{
    public:
        int start();
        void print_menu();
    private:
        char get_key();
};
