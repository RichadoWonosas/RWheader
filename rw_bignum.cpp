#include "rw_bignum.h"

namespace stdrw
{
    BigNumber::BigNumber() : length_(1), data_(new u64[1]{0}), symbol_(false) {}

    BigNumber::BigNumber(u64 *data, i32 length, bool symbol) : data_(data),
                                                               length_((length <= 0) ? 1 : length),
                                                               symbol_(symbol) {}

    BigNumber::BigNumber(const BigNumber &another) : length_(another.length_),
                                                     data_(new u64[another.length_]),
                                                     symbol_(another.symbol_)
    {
        i32 i;
        for (i = 0; i < length_; i++)
            data_[i] = another.data_[i];
    }

    BigNumber::BigNumber(BigNumber &&another) noexcept: length_(another.length_),
                                                        data_(another.data_),
                                                        symbol_(another.symbol_)
    {
        another.data_ = nullptr;
    }

    BigNumber::BigNumber(ibyte value) : length_(1),
                                        data_(new u64[1]{static_cast<u64>((value >= 0) ? value : -value)}),
                                        symbol_(value < 0) {}

    BigNumber::BigNumber(i16 value) : length_(1),
                                      data_(new u64[1]{static_cast<u64>((value >= 0) ? value : -value)}),
                                      symbol_(value < 0) {}

    BigNumber::BigNumber(i32 value) : length_(1),
                                      data_(new u64[1]{static_cast<u64>((value >= 0) ? value : -value)}),
                                      symbol_(value < 0) {}

    BigNumber::BigNumber(i64 value) : length_(1),
                                      data_(new u64[1]{static_cast<u64>((value >= 0) ? value : -value)}),
                                      symbol_(value < 0) {}

    BigNumber::BigNumber(byte value, bool symbol) : length_(1),
                                                    data_(new u64[1]{value}),
                                                    symbol_(symbol && value != 0) {}

    BigNumber::BigNumber(u16 value, bool symbol) : length_(1),
                                                   data_(new u64[1]{value}),
                                                   symbol_(symbol && value != 0) {}

    BigNumber::BigNumber(u32 value, bool symbol) : length_(1),
                                                   data_(new u64[1]{value}),
                                                   symbol_(symbol && value != 0) {}

    BigNumber::BigNumber(u64 value, bool symbol) : length_(1),
                                                   data_(new u64[1]{value}),
                                                   symbol_(symbol && value != 0) {}

    BigNumber::~BigNumber()
    {
        delete[] data_;
    }

    i32 BigNumber::length() const
    {
        return length_;
    }

    bool BigNumber::is_negative() const
    {
        return symbol_;
    }

    i64 BigNumber::get_total_bits() const
    {
        i64 result = (length_ - 1) << 6;

        for (u64 head = data_[length_ - 1]; head != 0; head >>= 1)
            result++;

        return result;
    }

    i64 BigNumber::get_tail_zero_length() const
    {
        i32 i;
        i64 result = 0;
        u64 digit;

        for (i = 0; i < length_ && data_[i] == 0; i++)
            result += 64;

        if (i < length_)
        {
            digit = data_[i];
            while ((digit & 1) == 0)
            {
                digit >>= 1;
                result++;
            }
        }

        return result;
    }

    void BigNumber::set_length(i32 length)
    {
        i32 i;
        if (length_ == length || length < 1)
            return;
        auto *new_data = new u64[length]{};

        if (data_ != nullptr)
        {
            if (length_ > length)
                length_ = length;
            for (i = 0; i < length_; i++)
                new_data[i] = data_[i];
            length_ = length;
            delete[] data_;
        }
        data_ = new_data;
    }

    void BigNumber::autofit_length()
    {
        i32 length = length_;
        while (length > 1 && data_[length - 1] == 0ull)
            --length;
        set_length(length);
        if (length == 1 && data_[0] == 0ull)
            set_symbol(SYMBOL_POSITIVE);
    }

    void BigNumber::set_symbol(bool symbol)
    {
        if (data_ == nullptr || length_ <= 0)
            symbol_ = false;
        else
        {
            if (length_ > 1 || data_[0] != 0ull)
                symbol_ = symbol;
            else
                symbol_ = false;
        }
    }

    BigNumber BigNumber::GetOnes(u64 digit)
    {
        if (digit == 0)
            return BigNumber();

        BigNumber result = BigNumber();
        result.set_length(((digit - 1) >> 6) + 1);
        for (auto i = 0; i < result.length_; i++)
            result.data_[i] = MAX_UINT64;

        if ((digit & 63) != 0)
            result.data_[result.length_ - 1] &= (1ull << (digit & 63)) - 1;
        return result;
    }

    u64 &BigNumber::operator[](i32 position) const
    {
        if (position >= length_)
            position = length_;
        else if (position < 0)
            position = 0;
        return data_[position];
    }

    BigNumber &BigNumber::operator=(const BigNumber &another)
    {
        i32 i;
        if (this == &another)
            return *this;


        delete[] data_;
        symbol_ = another.symbol_;
        length_ = another.length_;
        data_ = new u64[length_];

        for (i = length_ - 1; i >= 0; i--)
            data_[i] = another.data_[i];

        return *this;
    }

    BigNumber &BigNumber::operator=(BigNumber &&another) noexcept
    {
        if (this == &another)
            return *this;

        symbol_ = another.symbol_;
        length_ = another.length_;

        auto temp = data_;
        data_ = another.data_;
        another.data_ = temp;

        return *this;
    }

    bool BigNumber::operator==(const BigNumber &another) const
    {
        i32 i;
        if (length_ != another.length_ || symbol_ != another.symbol_)
            return false;
        for (i = length_ - 1; i >= 0; i--)
            if (data_[i] != another.data_[i])
                return false;
        return true;
    }

    bool BigNumber::operator!=(const BigNumber &another) const
    {
        i32 i;
        if (length_ != another.length_ || symbol_ != another.symbol_)
            return true;
        for (i = length_ - 1; i >= 0; i--)
            if (data_[i] != another.data_[i])
                return true;
        return false;
    }

    bool BigNumber::operator<(const BigNumber &another) const
    {
        i32 i;
        if (symbol_ ^ another.symbol_)
            return symbol_;

        if (length_ < another.length_)
            return !symbol_;
        if (length_ > another.length_)
            return symbol_;

        for (i = length_ - 1; i >= 0; i--)
        {
            if (data_[i] < another.data_[i])
                return !symbol_;
            if (data_[i] > another.data_[i])
                return symbol_;
        }
        return false;
    }

    bool BigNumber::operator>(const BigNumber &another) const
    {
        i32 i;
        if (symbol_ ^ another.symbol_)
            return !symbol_;

        if (length_ < another.length_)
            return symbol_;
        if (length_ > another.length_)
            return !symbol_;

        for (i = length_ - 1; i >= 0; i--)
        {
            if (data_[i] < another.data_[i])
                return symbol_;
            if (data_[i] > another.data_[i])
                return !symbol_;
        }
        return false;
    }

    bool BigNumber::operator<=(const BigNumber &another) const
    {
        i32 i;
        if (symbol_ ^ another.symbol_)
            return symbol_;

        if (length_ < another.length_)
            return !symbol_;
        if (length_ > another.length_)
            return symbol_;

        for (i = length_ - 1; i >= 0; i--)
        {
            if (data_[i] < another.data_[i])
                return !symbol_;
            if (data_[i] > another.data_[i])
                return symbol_;
        }
        return true;
    }

    bool BigNumber::operator>=(const BigNumber &another) const
    {
        i32 i;
        if (symbol_ ^ another.symbol_)
            return !symbol_;

        if (length_ < another.length_)
            return symbol_;
        if (length_ > another.length_)
            return !symbol_;

        for (i = length_ - 1; i >= 0; i--)
        {
            if (data_[i] < another.data_[i])
                return symbol_;
            if (data_[i] > another.data_[i])
                return !symbol_;
        }
        return true;
    }

    BigNumber BigNumber::operator-() const
    {
        auto result = BigNumber(*this);

        if (result.length() < 1 || (result.length() == 1 && result[0] == 0ull))
            return result;

        result.set_symbol(!result.is_negative());
        return result;
    }

    BigNumber BigNumber::operator<<(u64 bits) const
    {
        if (bits == 0)
            return *this;

        i32 i, shift_length = (i32) ((bits - 1) >> 6) + 1;
        byte single_bits = bits & 0b111111;

        auto result = BigNumber();

        result.symbol_ = symbol_;
        result.set_length(length_ + shift_length);

        for (i = 0; i < length_; i++)
        {
            result.data_[i + shift_length - 1] ^= (single_bits == 0) ? 0 : (data_[i] << single_bits);
            result.data_[i + shift_length] ^= data_[i] >> ((64 - single_bits) & 0b111111);
        }

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator>>(u64 bits) const
    {
        if (bits == 0)
            return *this;

        u64 digits = get_total_bits();
        if (digits <= bits)
            return BigNumber();

        i32 i, shift_length = ((i32) bits >> 6);
        byte single_bits = bits & 0b111111;

        auto result = BigNumber();
        result.symbol_ = symbol_;

        result.set_length(length_ - shift_length);
        for (i = 0; i < result.length_; i++)
            result.data_[i] ^= data_[i + shift_length] >> single_bits ^
                               ((i == result.length_ - 1 || single_bits == 0) ? 0 : (data_[i + shift_length + 1]
                                       << ((64 - single_bits) & 0b111111)));

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator+(const BigNumber &another) const
    {
        if (length_ < 1 || (length_ == 1 && data_[0] == 0ull))
            return another;
        if (another.length_ < 1 || (another.length_ == 1 && another.data_[0] == 0ull))
            return *this;

        if (symbol_ ^ another.symbol_)
            return *this - (-another);

        u64 carry = 0, middle, fin;
        i32 i;
        auto result = BigNumber(*this);

        result.set_length(((length_ >= another.length_) ? length_ : another.length_) + 1);

        for (i = 0; i < result.length_; i++)
        {
            middle = result.data_[i] + ((i < another.length_) ? another.data_[i] : 0ull);
            fin = middle + carry;
            carry = (((result.data_[i] ^ MAX_UINT64) < ((i < another.length_) ? another.data_[i] : 0ull)) ||
                     ((middle ^ MAX_UINT64) < carry)) ? 1 : 0;
            result.data_[i] = fin;
        }

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator-(const BigNumber &another) const
    {
        if (length_ < 1 || (length_ == 1 && data_[0] == 0ull))
            return -another;
        if (another.length_ < 1 || (another.length_ == 1 && another.data_[0] == 0ull))
            return *this;

        if (symbol_ != another.symbol_)
            return *this + (-another);

        if (*this == another)
            return BigNumber();
        if ((!symbol_ && *this < another) ||
            (symbol_ && *this > another))
            return -(another - *this);

        u64 borrow = 0, middle, fin;
        i32 i;
        auto result = BigNumber(*this);

        result.set_length(((length_ >= another.length_) ? length_ : another.length_));

        for (i = 0; i < result.length_; i++)
        {
            middle = result.data_[i] - ((i < another.length_) ? another.data_[i] : 0ull);
            fin = middle - borrow;
            borrow = (result.data_[i] < ((i < another.length_) ? another.data_[i] : 0ull) ||
                      middle < borrow) ? 1 : 0;
            result.data_[i] = fin;
        }

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator|(const BigNumber &another) const
    {
        auto result = BigNumber(*this);
        i32 i;

        result.set_length((length_ >= another.length_) ? length_ : another.length_);
        for (i = 0; i < another.length_; i++)
            result.data_[i] |= another[i];

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator&(const BigNumber &another) const
    {
        auto result = BigNumber(*this);
        i32 i;

        result.set_length((length_ >= another.length_) ? length_ : another.length_);
        for (i = 0; i < another.length_; i++)
            result.data_[i] &= another[i];

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator^(const BigNumber &another) const
    {
        auto result = BigNumber(*this);
        i32 i;

        result.set_length((length_ >= another.length_) ? length_ : another.length_);
        for (i = 0; i < another.length_; i++)
            result.data_[i] ^= another[i];

        result.autofit_length();
        return result;
    }

    BigNumber BigNumber::operator~() const
    {
        auto result = BigNumber(*this);
        i32 i;

        for (i = 0; i < result.length_; i++)
            result.data_[i] ^= MAX_UINT64;

        result.autofit_length();
        return result;
    }

    // TODO: times

    BigNumber BigNumber::operator/(const BigNumber &another) const
    {
        BigNumber dividend(*this), divider(another);
        BigNumber iter_divider, result(0);
        bool symbol = dividend.symbol_ != divider.symbol_;

        dividend.symbol_ = divider.symbol_ = SYMBOL_POSITIVE;
        if (dividend < divider || (divider.length_ == 1 && divider.data_[0] == 0))
            return BigNumber();

        i64 tail = divider.get_tail_zero_length();
        dividend >>= tail;
        divider >>= tail;

        if (divider.length_ == 1 && divider.data_[0] == 1)
        {
            dividend.symbol_ = symbol;
            return dividend;
        }

        u64 bit_diff = dividend.get_total_bits() - divider.get_total_bits();
        for (iter_divider = divider << bit_diff; iter_divider >= divider; iter_divider >>= 1)
        {
            result <<= 1;
            if (dividend >= iter_divider)
            {
                dividend -= iter_divider;
                result[0] ^= 1;
            }
        }

        result.symbol_ = symbol;
        return result;
    }

    BigNumber BigNumber::operator%(const BigNumber &another) const
    {
        BigNumber dividend(*this), divider(another);
        BigNumber iter_divider;
        bool symbol = dividend.symbol_;

        dividend.symbol_ = divider.symbol_ = SYMBOL_POSITIVE;
        if (dividend < divider)
            return symbol ? (divider - dividend) : dividend;
        else if (divider.length_ == 1 && divider.data_[0] == 0)
            return BigNumber();

        if (divider.get_total_bits() - divider.get_tail_zero_length() == 1)
            dividend &= GetOnes(divider.get_tail_zero_length());
        else
        {
            u64 bit_diff = dividend.get_total_bits() - divider.get_total_bits();
            for (iter_divider = divider << bit_diff; iter_divider >= divider; iter_divider >>= 1)
                if (dividend >= iter_divider)
                    dividend -= iter_divider;
        }

        if (symbol)
            dividend = divider - dividend;

        return dividend;
    }

    BigNumber &BigNumber::operator|=(const BigNumber &another)
    {
        *this = *this | another;
        return *this;
    }

    BigNumber &BigNumber::operator&=(const BigNumber &another)
    {
        *this = *this & another;
        return *this;
    }

    BigNumber &BigNumber::operator^=(const BigNumber &another)
    {
        *this = *this ^ another;
        return *this;
    }

    BigNumber &BigNumber::operator>>=(u64 bits)
    {
        *this = *this >> bits;
        return *this;
    }

    BigNumber &BigNumber::operator<<=(u64 bits)
    {
        *this = *this << bits;
        return *this;
    }

    BigNumber &BigNumber::operator+=(const BigNumber &another)
    {
        *this = *this + another;
        return *this;
    }

    BigNumber &BigNumber::operator-=(const BigNumber &another)
    {
        *this = *this - another;
        return *this;
    }

    BigNumber &BigNumber::operator*=(const BigNumber &another)
    {
        *this = *this * another;
        return *this;
    }

    BigNumber &BigNumber::operator/=(const BigNumber &another)
    {
        *this = *this / another;
        return *this;
    }

    BigNumber &BigNumber::operator%=(const BigNumber &another)
    {
        *this = *this % another;
        return *this;
    }

    BigNumber BigNumber::GCD(const BigNumber &another) const
    {
        BigNumber a(*this), b(another), c, zero(0);

        a.symbol_ = b.symbol_ = SYMBOL_POSITIVE;
        if (a < b)
        {
            c = a;
            a = b;
            b = c;
        }
        while (b > zero)
        {
            c = a % b;
            a = b;
            b = c;
        }

        return a;
    }

    BigNumber BigNumber::Inverse(const BigNumber &mod) const
    {
        if ((mod.length_ == 1 && mod.data_[0] == 0) || (length_ == 1 && data_[0] == 0))
            return BigNumber();
        else if ((mod.length_ == 1 && mod.data_[0] == 0) || (length_ == 1 && data_[0] == 1))
            return BigNumber(1);

        BigNumber a(*this), b(mod), c, div,
                abase(1), amod(0), bbase(0), bmod(1), cbase, cmod,
                zero(0);

        bool symbol = a.symbol_;
        a.symbol_ = b.symbol_ = SYMBOL_POSITIVE;

        while (b > zero)
        {
            c = a % b;
            div = a / b;
            cbase = abase - div * bbase;
            cmod = amod - div * bmod;
            a = b;
            abase = bbase;
            amod = bmod;
            b = c;
            bbase = cbase;
            bmod = cmod;
        }

        abase.symbol_ = symbol;
        return abase;
    }

    BigNumber BigNumber::Power(const BigNumber &power) const
    {
        BigNumber result(1);
        i64 i;

        if (power <= BigNumber())
            return result;

        for (i = power.get_total_bits(); i >= 0; i--)
        {
            result *= result;
            if (((power.data_[i >> 6] >> (i & 63)) & 1) == 1)
                result *= *this;
        }

        return result;
    }

    BigNumber BigNumber::Power(const BigNumber &power, const BigNumber &mod) const
    {
        BigNumber result(1);
        i64 i;

        if (power <= BigNumber())
            return result;

        for (i = power.get_total_bits(); i >= 0; i--)
        {
            result *= result;
            if (((power.data_[i >> 6] >> (i & 63)) & 1) == 1)
                result *= *this;

            result %= mod;
        }

        return result;
    }

    bool BigNumber::IsPrime() const
    {
        if (length_ == 1 && data_[0] == 2)
            return true;
        else if ((length_ == 1 && (data_[0] == 0 || data_[0] == 1)) || (data_[0] & 1) == 0)
            return false;

        static const u64 tests[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
                                      79, 83, 89, 97, 101};

        BigNumber one(1);
        BigNumber original(*this);
        original.symbol_ = SYMBOL_POSITIVE;

        BigNumber phi(original - one);
        i64 tail = phi.get_tail_zero_length(), j;
        BigNumber odd(phi >> tail);

        int i;

        for (i = 0; i < 26; i++)
        {
            if (length_ == 1 && data_[0] == tests[i])
                return true;

            BigNumber test(BigNumber(tests[i]).Power(odd, original));
            if (test == one)
                continue;
            for (j = 0; j < tail; j++)
            {
                if (test == phi)
                    break;
                else if (test == one)
                    return false;
                test = (test * test) % original;
            }
            if (j == tail)
                return false;
        }

        return true;
    }
}