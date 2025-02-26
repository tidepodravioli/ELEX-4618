/**
 * @file CGrocery.h
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Class representing a list of CProducts
 * @version 0.1
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

// libraries
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include "tinyxml/tinyxml.h"

// project files
#include "getstuff.h"
#include "CProduct.h"

#define EMPTY_STRING ""

// regex for filename input
#define FILENAME_NO_EXTENSION_REGEX "^[A-Za-z0-9]+$"
#define FILENAME_WITH_EXTENSION_REGEX "^[A-Za-z0-9]+.xml$"

// column widths for 'cli_printProducts()'
#define PRODUCT_PADDING 29
#define PRICE_PADDING 15
#define QUANTITY_PADDING 15
#define VALUE_PADDING 15

// xml file tag/element names
#define FILE_ROOT "grocery"

#define ENTRY_ITEM "item"

#define ATTR_NAME "name"
#define ATTR_PRICE "price"
#define ATTR_QUANTITY "quantity"



using namespace std;

class CGrocery
{
    public:
        int get_size(); //gets amount of products in the grocery list
        float get_value(); //gets total value of all products in the grocery list

        // User interaction through CLI
        void cli_addProduct();
        void cli_editProduct();
        void cli_printProducts();
        void cli_deleteProduct();
        void cli_loadFromFile();
        void cli_saveToFile();
    private:
        vector<CProduct> m_products;

        int cli_getIndex(); // gets an index from the user (with error rejection)
        bool cli_getFileName(string * result, bool checkIfExists = true);

        void deleteAllProducts(); // deletes all the products in the grocery list

        string priceToString(float price); //converts float to 2 decimal place price
};
