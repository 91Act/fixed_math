#pragma once
#include "fixed_math.h"

class fixed_number
{
public:
    DEPRECATED("Float may bring uncertain value on different platforms. Please avoid using this to calculate critical values")
    constexpr explicit fixed_number(const double& _data)
        : data(fixed_dtox(_data))
    {}

    DEPRECATED("Float may bring uncertain value on different platforms. Please avoid using this to calculate critical values")
    constexpr explicit fixed_number(const float& _data)
        : data(fixed_dtox(_data))
    {}

    constexpr explicit fixed_number(const int& _data)
        : data(fixed_itox(_data))
    {}

    constexpr fixed_number()
        : data(0)
    {}

    constexpr fixed_number(const fixed_number& other)
        : data(other.data)
    {}

    constexpr static fixed_number make(const int& ival)
    {
        return fixed_number(fixed_itox(ival));
    }

    fixed_number& operator +=(const fixed_number& other)
    {
        data = fixed_add(data, other.data);
        return *this;
    }

    fixed_number& operator -=(const fixed_number& other)
    {
        data = fixed_sub(data, other.data);
        return *this;
    }

    fixed_number& operator *=(const fixed_number& other)
    {
        data = fixed_mul(data, other.data);
        return *this;
    }

    fixed_number& operator /=(const fixed_number& other)
    {
        data = fixed_div(data, other.data);
        return *this;
    }

    fixed_number& operator %=(const fixed_number& other)
    {
        data = fixed_mod(data, other.data);
        return *this;
    }

    fixed_number operator+ (const fixed_number& other) const
    {
        return fixed_number(data) += other;
    }

    fixed_number operator- (const fixed_number& other) const
    {
        return fixed_number(data) -= other;
    }

    fixed_number operator* (const fixed_number& other) const
    {
        return fixed_number(data) *= other;
    }

    fixed_number operator/ (const fixed_number& other) const
    {
        return fixed_number(data) /= other;
    }

    fixed_number operator% (const fixed_number& other) const
    {
        return fixed_number(data) %= other;
    }

    constexpr fixed_number operator -() const
    {
        return fixed_number(-data);
    }

    constexpr bool operator > (const fixed_number& other) const
    {
        return data > other.data;
    }

    constexpr bool operator >= (const fixed_number& other) const
    {
        return data >= other.data;
    }

    constexpr bool operator < (const fixed_number& other) const
    {
        return data < other.data;
    }

    constexpr bool operator <= (const fixed_number& other) const
    {
        return data <= other.data;
    }

    constexpr bool operator == (const fixed_number& other) const
    {
        return data == other.data;
    }

    constexpr bool operator != (const fixed_number& other) const
    {
        return data != other.data;
    }

    bool isinf() const
    {
        return fixed_isinf(data);
    }

    bool isnan() const
    {
        return fixed_isnan(data);
    }

    fixed_number floor() const
    {
        return fixed_floor(data);
    }

    fixed_number ceil() const
    {
        return fixed_ceil(data);
    }

    fixed_number abs() const
    {
        return fixed_abs(data);
    }

    fixed_number sqrt() const
    {
        return fixed_sqrt(data);
    }

    fixed_number pow(const fixed_number& other) const
    {
        return fixed_pow(data, other.data);
    }

    fixed_number sin() const
    {
        return fixed_sin(data);
    }

    fixed_number cos() const
    {
        return fixed_cos(data);
    }

    fixed_number tan() const
    {
        return fixed_tan(data);
    }

    fixed_number asin() const
    {
        return fixed_asin(data);
    }

    fixed_number acos() const
    {
        return fixed_acos(data);
    }

    fixed_number atan() const
    {
        return fixed_atan(data);
    }

    fixed_number atan(const fixed_number& x) const
    {
        return fixed_atan2(data, x.data);
    }

    fixed_number exp() const
    {
        return fixed_exp(data);
    }

    fixed_number log() const
    {
        return fixed_log(data);
    }

    constexpr explicit operator float() const
    {
        return fixed_xtod(data);
    }

    constexpr explicit operator int() const
    {
        return fixed_xtoi(data);
    }

public:
    static const fixed_number max;
    static const fixed_number epsilon;
    static const fixed_number pi;

private:
    constexpr fixed_number(const fixed_point& _data)
        : data(_data)
    {}

    friend constexpr fixed_number operator"" _fx(unsigned long long data);
    friend constexpr fixed_number operator"" _fx(const char* str, size_t len);

private:
    fixed_point data;
};


constexpr fixed_number operator"" _fx(unsigned long long data)
{
    return fixed_number(fixed_itox(data));
}

constexpr fixed_number operator"" _fx(const char* str, size_t len)
{
    return fixed_number(fixed_natox(str, len));
}
