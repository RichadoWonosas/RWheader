#include "rw_bignum.h"
#include <iostream>
#include <iomanip>
#include <ctime>

int trial4()
{
    long i, j;
    stdrw::u64 value[2];
    stdrw::u64 value2[3] = {0xffffffffffffffffull, 0x200000001ull, 0x1ull};

    std::cout << "Please enter values: ";
    std::cin >> value[0] >> value[1];
    std::cout << "\n";

    stdrw::BigNumber a(value[0]);
    stdrw::BigNumber b(value[1]);
    //    auto c = a - b;
    stdrw::BigNumber d[] = {
        //            c,
        //            (a - c) - -(b - c),
        //            (a - c) - (b - c),
        //            b - -a - -c,
        //            (a - b - -c) << 128,
        //            ((a - -b - -c) << 128) >> 64,
        //            (a - -b - -c) << 130,
        //            ((a - -b - -c) << 130) >> 66,
        //            a | b,
        //            a & b,
        //            a ^ b,
        //            ~a,
        stdrw::BigNumber::GetOnes(1024) * stdrw::BigNumber::GetOnes(1024),
        stdrw::BigNumber::GetOnes(1000000000) - stdrw::BigNumber::GetOnes(500000000),
        stdrw::BigNumber::GetOnes(100000000) - stdrw::BigNumber::GetOnes(50000000),
        stdrw::BigNumber::GetOnes(10000000) - stdrw::BigNumber::GetOnes(5000000),
        stdrw::BigNumber::GetOnes(1000000) - stdrw::BigNumber::GetOnes(500000),
        stdrw::BigNumber::GetOnes(100000) - stdrw::BigNumber::GetOnes(50000),
        stdrw::BigNumber::GetOnes(10000) - stdrw::BigNumber::GetOnes(5000),
        stdrw::BigNumber::GetOnes(1000) - stdrw::BigNumber::GetOnes(500),
        stdrw::BigNumber::GetOnes(100) - stdrw::BigNumber::GetOnes(50),
        stdrw::BigNumber::GetOnes(10) - stdrw::BigNumber::GetOnes(5)};

    // int round = 10;
    // for (j = 0; j < 9; j++)
    // {
    //     auto st = clock();
    //     for (i = 1; i <= round; i++)
    //     {
    //         d[j] += d[j];
    //         d[j] >>= 1;
    //     }
    //     std::cout << j << " : " << clock() - st << std::endl;

    //     round *= 10;
    //     std::cout << std::endl;
    // }

    for (i = 0; i < 2; i++)
    {
        std::cout << std::dec << d[i].get_total_bits() << "\n";
        std::cout << (d[i].is_negative() ? "-" : "");
        for (j = ((d[i].length() - 1) > 64) ? 64 : (d[i].length() - 1); j >= 0; j--)
            std::cout << std::hex << std::setw(16) << std::setfill('0') << d[i][j] << " ";

        std::cout << "\n\n";
    }

    std::cout << "a == b ? " << ((a == b) ? "true" : "false") << std::endl;
    std::cout << "a != b ? " << ((a != b) ? "true" : "false") << std::endl;
    std::cout << "a < b  ? " << ((a < b) ? "true" : "false") << std::endl;
    std::cout << "a > b  ? " << ((a > b) ? "true" : "false") << std::endl;
    std::cout << "a <= b ? " << ((a <= b) ? "true" : "false") << std::endl;
    std::cout << "a >= b ? " << ((a >= b) ? "true" : "false") << std::endl;
    // std::cout << "b prime ? " << (b.IsPrime() ? "true" : "false") << std::endl;

    return 0;
}
