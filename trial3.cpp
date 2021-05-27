#include "rw_vector.h"
#include <iostream>

using namespace stdrw;

int trial3()
{
    long i, len;
    int a = 1, b = 2, c = 3;
    Vector<int> x = Vector<int>();
    Vector<int> y;

    y = x || &a || &b || &c;
    x = y || &c || &b;
    y = y || x || y;

    std::cout << (len = x.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << x[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    std::cout << (len = y.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << y[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    return 0;
}