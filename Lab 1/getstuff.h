/**
 * @file getstuff.h
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Header file for 'getstuff.cpp'
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <regex>
#include <string>

#define MAX_CHAR_ARRAY 128

#define IGNORE_NUM 10000
#define IGNORE_DELIM '\n'

#define CHAR_REGEX "^.$"
#define INTEGER_REGEX "^[0-9]+$"
#define FLOAT_REGEX "^[0-9]+(|\\.([0-9]{1,2}|))$"

bool get_int(int * result);
bool get_float(float * result);
bool get_char(char * result);

bool get_line(std::string * result);
bool get_data(std::string * result, std::regex data);

void get_reset();