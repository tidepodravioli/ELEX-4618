/**
 * @file main.cpp
 * @author Rafael Banalan   A01367816 4S (abanalan@my.bcit.ca)
 * @brief Contains functions that Holds main(), print_menu() and get_key() functions.
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include "CGrocery.h"
#include "getstuff.h"

#define TO_LOWERCASE 32 //number to add to uppercase char to make it lowercase

void print_menu();
char get_key();

int main()
{
    char keyPressed;
    std::cout << std::fixed << std::setprecision(2);
    CGrocery inventory;
    
    do
    {
        print_menu();
        keyPressed = get_key();

        switch(keyPressed)
        {
            case 'a':
            inventory.cli_addProduct();
            break;

            case 'e':
            inventory.cli_editProduct();
            break;

            case 'p':
            inventory.cli_printProducts();
            break;
        }

    } while (keyPressed != 'q');

    std::cout << "Exiting...";
    return 0;
}

void print_menu()
{
    std::cout << "*******************************************" << '\n';
    std::cout << "ELEX4618 Grocery System, by Rafael Banalan" << '\n';
    std::cout << "*******************************************" << '\n';
    std::cout << "(A)dd product" << '\n';
    std::cout << "(E)dit product" << '\n';
    std::cout << "(P)rint products" << '\n';
    std::cout << "(Q)uit" << '\n';
    std::cout << "CMD> " << std::flush;
}

char get_key()
{
    char keyPressed;
    get_char(&keyPressed);

    //if keyPressed is an uppercase letter, make it lowercase for the switch conditions
    if(keyPressed >= 'A' && keyPressed <= 'Z')
        keyPressed += TO_LOWERCASE;

    return keyPressed;
}