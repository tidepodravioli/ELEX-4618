/**
 * @file CGroceryManager.cpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Implementation of CGroceryManager.h
 * @version 0.1
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CGroceryManager.h"

int CGroceryManager::start()
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

            case 'd':
            inventory.cli_deleteProduct();
            break;

            case 'p':
            inventory.cli_printProducts();
            break;

            case 's':
            inventory.cli_saveToFile();
            break;

            case 'l':
            inventory.cli_loadFromFile();
            break;

            case 'q':
            break;

            default:
            cout << "Invalid command. See menu.\n";
            break;
        }

    } while (keyPressed != 'q');

    std::cout << "Exiting...";
    return 0;
}

void CGroceryManager::print_menu()
{
    std::cout << "*******************************************" << '\n';
    std::cout << "ELEX4618 Grocery System, by Rafael Banalan" << '\n';
    std::cout << "*******************************************" << '\n';
    std::cout << "(A)dd product" << '\n';
    std::cout << "(E)dit product" << '\n';
    std::cout << "(D)elete product" << '\n';
    std::cout << "(P)rint products" << '\n';
    std::cout << "(S)ave inventory to file" << '\n';
    std::cout << "(L)oad inventory to file" << '\n';
    std::cout << "(Q)uit" << '\n';
    std::cout << "CMD> " << std::flush;
}

char CGroceryManager::get_key()
{
    char keyPressed;
    get_char(&keyPressed);

    //if keyPressed is an uppercase letter, make it lowercase for the switch conditions
    if(keyPressed >= 'A' && keyPressed <= 'Z')
        keyPressed += TO_LOWERCASE;

    return keyPressed;
}