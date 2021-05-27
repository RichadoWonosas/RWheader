#include "rw_vector.h"
#include "rw_bignum.h"
#include <iostream>

int trial1()
{
    long i, len;
    stdrw::Vector<int> x(nullptr);
    stdrw::Vector<int> y;
    stdrw::Vector<int> z(12, 5);
    stdrw::Vector<int> a = stdrw::Vector<int>(z);

    x.Append(1);
    x.Insert(2, 1);

    std::cout << (len = x.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << x[i] << " " << x.ElementAt(i) << "\n";

    y = x;

    std::cout << (len = y.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << y[i] << " " << y.ElementAt(i) << "\n";

    std::cout << ((y == x) ? "true" : "false") << "\n\n";

    std::cout << (len = z.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << z[i] << " " << z.ElementAt(i) << "\n\n";

    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " " << a.ElementAt(i) << "\n";
    std::cout << a[5] << "\n\n";

    std::cout << y.Pop() << "\n";
    std::cout << (len = y.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << y[i] << " " << y.ElementAt(i) << "\n\n";

    std::cout << x.Dequeue() << "\n";
    std::cout << (len = x.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << x[i] << " " << x.ElementAt(i) << "\n\n";

    return 0;
}