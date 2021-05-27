#pragma once

#ifndef __RW_BIGNUM__
#define __RW_BIGNUM__ 100

#include <cstdint>

namespace stdrw
{
    using ibyte = int8_t;
    using byte = uint8_t;
    using i16 = int16_t;
    using u16 = uint16_t;
    using i32 = int32_t;
    using u32 = uint32_t;
    using i64 = int64_t;
    using u64 = uint64_t;

    class BigNumber
    {
    private:
        u64 *data_;
        bool symbol_;
        i32 length_;

        void set_length(i32 length);

        void autofit_length();

        i64 get_tail_zero_length() const;

    public:
        constexpr static byte Base64[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

        static const bool SYMBOL_POSITIVE = false;
        static const bool SYMBOL_NEGATIVE = true;
        static const byte MID_UINT8 = (static_cast<byte>(-1) >> 1) + 1;
        static const byte MAX_UINT8 = static_cast<byte>(-1);
        static const u16 MID_UINT16 = (static_cast<u16>(-1) >> 1) + 1;
        static const u16 MAX_UINT16 = static_cast<u16>(-1);
        static const u32 MID_UINT32 = (static_cast<u32>(-1) >> 1) + 1;
        static const u32 MAX_UINT32 = static_cast<u32>(-1);
        static const u64 MID_UINT64 = (static_cast<u64>(-1) >> 1) + 1;
        static const u64 MAX_UINT64 = static_cast<u64>(-1);

        i64 get_total_bits() const;

        BigNumber();

        BigNumber(u64 *data, i32 length, bool symbol = true);

        BigNumber(const BigNumber &another);

        BigNumber(BigNumber &&another) noexcept;

        explicit BigNumber(ibyte value);

        explicit BigNumber(i16 value);

        explicit BigNumber(i32 value);

        explicit BigNumber(i64 value);

        explicit BigNumber(byte value, bool symbol = SYMBOL_POSITIVE);

        explicit BigNumber(u16 value, bool symbol = SYMBOL_POSITIVE);

        explicit BigNumber(u32 value, bool symbol = SYMBOL_POSITIVE);

        explicit BigNumber(u64 value, bool symbol = SYMBOL_POSITIVE);

        ~BigNumber();

        i32 length() const;

        bool is_negative() const;

        void set_symbol(bool symbol);

        static BigNumber GetOnes(u64 digit);

        u64 &operator[](i32 position) const;

        BigNumber &operator=(const BigNumber &another);

        BigNumber &operator=(BigNumber &&another) noexcept;

        bool operator==(const BigNumber &another) const;

        bool operator!=(const BigNumber &another) const;

        bool operator<(const BigNumber &another) const;

        bool operator>(const BigNumber &another) const;

        bool operator<=(const BigNumber &another) const;

        bool operator>=(const BigNumber &another) const;

        BigNumber operator-() const;

        BigNumber operator|(const BigNumber &another) const;

        BigNumber operator&(const BigNumber &another) const;

        BigNumber operator^(const BigNumber &another) const;

        BigNumber operator~() const;

        BigNumber operator>>(u64 bits) const;

        BigNumber operator<<(u64 bits) const;

        BigNumber operator+(const BigNumber &another) const;

        BigNumber operator-(const BigNumber &another) const;

        BigNumber operator*(const BigNumber &another) const;

        BigNumber operator/(const BigNumber &another) const;

        BigNumber operator%(const BigNumber &another) const;

        BigNumber &operator|=(const BigNumber &another);

        BigNumber &operator&=(const BigNumber &another);

        BigNumber &operator^=(const BigNumber &another);

        BigNumber &operator>>=(u64 bits);

        BigNumber &operator<<=(u64 bits);

        BigNumber &operator+=(const BigNumber &another);

        BigNumber &operator-=(const BigNumber &another);

        BigNumber &operator*=(const BigNumber &another);

        BigNumber &operator/=(const BigNumber &another);

        BigNumber &operator%=(const BigNumber &another);

        BigNumber GCD(const BigNumber &another) const;

        BigNumber Inverse(const BigNumber &base) const;

        BigNumber Power(const BigNumber &power) const;

        BigNumber Power(const BigNumber &power, const BigNumber &mod) const;

        bool IsPrime() const;
    };
} // namespace stdrw

#endif // __RW_BIGNUM__