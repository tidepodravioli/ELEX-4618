/**
 * @file getstuff.cpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Contains functions that are related to retrieving user input.
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "getstuff.h"

bool get_int(int * result)
{
    std::string input;
    if(get_data(&input, std::regex(INTEGER_REGEX)))
    {
        * result = std::stoi(input);
        return true;
    }

    return false;
}

bool get_float(float * result)
{
    std::string input;
    if(get_data(&input, std::regex(FLOAT_REGEX)))
    {
        * result = std::stof(input);
        return true;
    }

    return false;
}

bool get_char(char * result)
{
    std::string input;
    if(get_data(&input, std::regex(CHAR_REGEX)))
    {
        * result = input[0];
        return true;
    }
    return false;
}

bool get_line(std::string * result)
{
    std::getline(std::cin, * result);
    return !std::cin.fail();
}

bool get_data(std::string * result, std::regex data)
{
    if(get_line(result)){
        return(std::regex_match(* result, data));
    }
    else return false;
}

void get_reset()
{
    std::cin.clear();
    std::cin.ignore(IGNORE_NUM, IGNORE_DELIM);
}