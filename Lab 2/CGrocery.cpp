/**
 * @file CGrocery.cpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief Implementation of CGrocery.h
 * @version 0.1
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CGrocery.h"

///////////////////////
// PUBLIC FUNCTIONS //
///////////////////////

int CGrocery::get_size()
{
    return m_products.size();
}

float CGrocery::get_value()
{
    float value = 0;
    for(int index = 0; index < get_size(); index++)
    {
        value += m_products[index].get_value();
    }

    return value;
}

void CGrocery::cli_addProduct()
{
    cout << "Adding product" << endl;
    cout << "Name of product:  " ;

    string productName;
    while(!get_data(&productName, regex(NON_BLANK_STRING_REGEX)))
    {
        cout << "Product name cannot be blank, begin with whitespace, or contain special characters." << endl;
        cout << "Name of product:  " ;
    }

    CProduct newProduct(productName);
    m_products.push_back(newProduct);
}

void CGrocery::cli_editProduct()
{
    if(get_size() != 0)
    {
        cout << "Index of product to edit :  " ;
        int productIndex = cli_getIndex();

        cout << "Product name : " << m_products[productIndex].get_name() << endl;

        cout << "Product price :  ";
        float price;
        while(!get_float(&price))
        {
            cout << "Please enter a valid price!" << endl;
            cout << "Product price :  " ;
        }
        m_products[productIndex].set_price(price);

        cout << "Product quantity :  ";
        int quantity;
        while(!get_int(&quantity))
        {
            cout << "Quantity must be an integer." << endl;
            cout << "Product quantity :  " ;
        }
        m_products[productIndex].set_quantity(quantity);
    }
    else
    {
        cout << "There are no products in the inventory!" << endl;
        cout << "Please add products or import a file with products to edit." << endl;
    }
}

void CGrocery::cli_printProducts()
{
    if(get_size() != 0)
    {
        cout << endl;
        cout << setw(PRODUCT_PADDING) << left << "Product";
        cout << setw(PRICE_PADDING) << "Price";
        cout << setw(QUANTITY_PADDING) << "Quantity";
        cout << setw(VALUE_PADDING) << "Value" << endl;

        for(int index = 0; index < get_size(); index++)
        {
            stringstream productColumn, priceColumn, valueColumn;
            
            productColumn << "(" << index + 1 << ")" << m_products[index].get_name();
            priceColumn << "$" << m_products[index].get_price();
            valueColumn << "$" << m_products[index].get_value();

            cout << setw(PRODUCT_PADDING) << left << productColumn.str();
            cout << setw(PRICE_PADDING) << priceColumn.str();
            cout << setw(QUANTITY_PADDING) << m_products[index].get_quantity();
            cout << setw(VALUE_PADDING) << valueColumn.str();
            cout  << endl;
        }


        cout << "Total : $" << get_value() << endl;
    }
    else
    {
        cout << "There are no products in the inventory!" << endl;
        cout << "Please add products or import a file." << endl;
    }
    
}

void CGrocery::cli_deleteProduct()
{
    if(get_size() != 0)
    {
        cout << "Enter index of product to delete: ";
        int productIndex = cli_getIndex();

        cout << "Product \"" << m_products[productIndex].get_name() << "\" deleted." << endl;
        m_products.erase(m_products.begin() + productIndex);
    }
    else
    {
        cout << "There are no products in the inventory!" << endl;
        cout << "Please add products or import a file with products to delete." << endl;
    }
}

void CGrocery::cli_loadFromFile()
{
    string fileName;
    if(!cli_getFileName(&fileName))
        return;
    
    TiXmlDocument file(fileName.c_str());
    if(!file.LoadFile())
    {
        cout << "File is invalid." << endl;
        return;
    }
    
    TiXmlHandle documentHandle(&file);
    TiXmlElement * firstElement = documentHandle.FirstChildElement(FILE_ROOT).Element();
    TiXmlHandle grocery(firstElement);

    if(!firstElement)
    {
        cout << "File is missing root element. File cannot be parsed." << endl;
        return;
    }

    deleteAllProducts();

    TiXmlElement * item;
    for(int itemIndex = 0; true; itemIndex++)
    {
        /*
            These next two lines of code get the i'th element inside the "grocery" tag.
            The if statement breaks the loop if there are no elements inside the tag
            or if every element inside the tag has already been parsed.
        */
        item = grocery.Child(itemIndex).Element();
        if(!item) break;
        string elementName = item->Value();
        if(!elementName.compare(ENTRY_ITEM))
        {
            try
            {
                string name = item ->Attribute(ATTR_NAME);
                float price = stof(item->Attribute(ATTR_PRICE));
                int quantity = stoi(item->Attribute(ATTR_QUANTITY));

                
                CProduct toAdd(name, price, quantity);
                m_products.push_back(toAdd);
            }
            catch(const exception& exception)
            {
                cout << "A problem occured while parsing the file!" << endl;
                return;
            }
        }
        else
        {
            cout << item->Value() << endl;
        }
    }

    cout << "File successfully loaded!" << endl;
}

void CGrocery::cli_saveToFile()
{
    string filename;
    if(!cli_getFileName(&filename, false))
        return;

    TiXmlDocument saveFile;
    TiXmlElement * root = new TiXmlElement(FILE_ROOT);
    saveFile.LinkEndChild(root);

    for(int itemIndex = 0; itemIndex < get_size(); itemIndex++)
    {
        TiXmlElement * item = new TiXmlElement(ENTRY_ITEM);
        root->LinkEndChild(item);

        item->SetAttribute(ATTR_NAME, m_products[itemIndex].get_name().c_str());
        item->SetAttribute(ATTR_PRICE, priceToString(m_products[itemIndex].get_price()).c_str());
        item->SetAttribute(ATTR_QUANTITY, m_products[itemIndex].get_quantity());
    }

    saveFile.SaveFile(filename.c_str());
    cout << "File saved successfully." << endl;
}

///////////////////////
// PRIVATE FUNCTIONS //
///////////////////////

int CGrocery::cli_getIndex()
{
    int productIndex;
    while(!get_int(&productIndex) || productIndex > get_size() || productIndex <= 0)
    {
        cout << "Please enter a valid product index!" << endl;
        cout << "Product to edit :  " ;
    }

    return productIndex - 1;
}

bool CGrocery::cli_getFileName(string * result, bool checkIfExists)
{
    cout << "Enter file name : ";

    string fileName;
    get_line(&fileName);

    if(regex_match(fileName, regex(FILENAME_NO_EXTENSION_REGEX)))
        fileName.append(".xml");
    else if(regex_match(fileName, regex(FILENAME_WITH_EXTENSION_REGEX)));
    else
    {
        cout << "Please enter a valid file name! (only alphanumeric characters, ending with or without .xml)" << endl;
        return false;
    }

    if(checkIfExists)
    {
        fstream check(fileName);
        if(!check || !check.is_open())
        {
            cout << "File could not be accessed. Please enter a valid file name!" << endl;
            return false;
        }
    }

    * result = fileName;
    return true; 
}

void CGrocery::deleteAllProducts()
{
    m_products.clear();
}

string CGrocery::priceToString(float price)
{
    stringstream converter;
    converter << fixed << setprecision(2) << price;
    return converter.str();
}