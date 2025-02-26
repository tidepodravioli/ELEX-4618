/**
 * @file CProduct.cpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Implementation of CProduct.h
 * @version 0.1
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CProduct.h"

CProduct::CProduct(string name)
{
    m_name = name;
    m_price = 0;
    m_quantity = 0;
}

CProduct::CProduct(string name, float price)
{
    m_name = name;
    m_price = price;
    m_quantity = 0;
}

CProduct::CProduct(string name, float price, int quantity)
{
    m_name = name;
    m_price = price;
    m_quantity = quantity;
}

string CProduct::get_name()
{
    return m_name;
}

void CProduct::set_name(string name)
{
    m_name = name;
}

float CProduct::get_price()
{
    return m_price;
}
void CProduct::set_price(float price)
{
    m_price = price;
}

int CProduct::get_quantity()
{
    return m_quantity;
}

void CProduct::set_quantity(int quantity)
{
    m_quantity = quantity;
}

float CProduct::get_value()
{
    return m_price * m_quantity;
}