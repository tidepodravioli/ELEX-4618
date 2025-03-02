#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "getstuff.h"
#include "CProduct.h"

using namespace std;

class CGrocery
{
    private:
        vector<CProduct> m_products;
        int getIndex();
    public:
        int get_size();
        void cli_addProduct();
        void cli_editProduct();
        void cli_printProducts();
};