#include "CGrocery.h"

int CGrocery::get_size()
{
    return m_products.size();
}

void CGrocery::cli_addProduct()
{
    cout << "Adding product" << '\n';
    cout << "Name of product:  " ;

    string productName;
    get_line(&productName);

    CProduct newProduct;
    newProduct.name = productName;
    newProduct.price = 0.0;
    newProduct.quantity = 0;
    
    m_products.push_back(newProduct);
}

void CGrocery::cli_editProduct()
{
    if(m_products.size() != 0)
    {
        cout << "Index of product to edit :  " ;
        int productIndex;
        productIndex = CGrocery::getIndex() - 1;

        cout << "Product name : " << m_products[productIndex].name << '\n';

        cout << "Product price :  ";
        float * price = &m_products[productIndex].price;
        while(!get_float(price))
        {
            cout << "Please enter a valid price!\n";
            cout << "Product price :  " ;
        }

        cout << "Product quantity :  ";
        while(!get_int(&m_products[productIndex].quantity))
        {
            cout << "Quantity must be an integer.\n";
            cout << "Product quantity :  " ;
        }
    }
    else
    {
        cout << "There are no products in the inventory!\n";
        cout << "Please add products or import a file with products to edit.\n";
    }
}

void CGrocery::cli_printProducts()
{
    cout << '\n' << "Product\t\t\t\t" << "Price\t\t" <<  "Quantity\t"  << "Value" << '\n';
    float total = 0;
    for(int i = 0; i < m_products.size(); i++)
    {
        cout << '(' << i+1 << ')';
        cout << setw(29) << left << m_products[i].name;
        cout << "$" << setw(15) << m_products[i].price;
        cout << setw(15) << m_products[i].quantity;

        float value = m_products[i].price * m_products[i].quantity;
        total += value;
        cout << "\t$" << value;

        cout << setw(0) << '\n';
    }

    cout << "Total : $" << total << '\n';
}

int CGrocery::getIndex()
{
    int productIndex;
    while(!get_int(&productIndex) || productIndex > m_products.size() || productIndex <= 0)
    {
        cout << "Please enter a valid product index!\n";
        cout << "Product to edit :  " ;
    }

    return productIndex;
}