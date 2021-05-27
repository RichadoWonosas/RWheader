#include "rw_vector.h"
#include <iostream>

using namespace stdrw;

int trial2()
{
    long i, len;
    Vector<int> a = Vector<int>();
    a.Append(1);
    a.Append(2);
    a.Append(3);
    a.Append(4);
    a.Append(5);
    a.Append(4);
    a.Append(6);
    a.Append(3);

    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    a.Sort();
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    a.Sort(ORDER_SMALL2BIG);
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    a.Sort(ORDER_BIG2SMALL);
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    std::cout << a.Pop() << "\n";
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    std::cout << a.Dequeue() << "\n";
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    std::cout << a.DeleteElementAt(3) << "\n";
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    std::cout << a.DeleteElementWithValue(3) << "\n";
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    std::cout << a.DeleteAllElementsWithValue(4) << "\n";
    std::cout << (len = a.get_element_amount()) << "\n";
    for (i = 0; i < len; i++)
        std::cout << a[i] << " ";
    std::cout << "\n";

    std::cout << "\n";

    return 0;
}