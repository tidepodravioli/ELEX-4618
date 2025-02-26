/**
 * @file CProduct.h
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Represents a product in a grocery inventory.
 * @version 0.1
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class CProduct
{
    public:
        // constructors
        CProduct(string name);
        CProduct(string name, float price);
        CProduct(string name, float price, int quantity);

        string get_name();
        void set_name(string name);

        float get_price();
        void set_price(float price);

        int get_quantity();
        void set_quantity(int quantity);

        float get_value(); //calculated at runtime

    private:
        string m_name;
        float m_price;
        int m_quantity;
};