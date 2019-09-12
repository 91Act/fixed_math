#include "fixed_math.h"
#include "fixed_lut.h"

#ifdef __SIZEOF_INT128__
typedef __int128 large_int;
#else
#define BOOST_NO_CXX17_HDR_STRING_VIEW
#include <boost/multiprecision/cpp_int.hpp>
typedef boost::multiprecision::int128_t large_int;
#endif

fixed_point fixed_sign(fixed_point value)
{
    return value >= 0 ? fixed_one : -fixed_one;
}

int fixed_isinf(fixed_point value)
{
    return (fixed_abs(value) & fixed_inf) == fixed_inf;
}

int fixed_isnan(fixed_point value)
{
    return value == fixed_nan;
}

fixed_point fixed_add(fixed_point value_a, fixed_point value_b)
{
    return value_a + value_b;
}

fixed_point fixed_sub(fixed_point value_a, fixed_point value_b)
{
    return value_a - value_b;
}

fixed_point fixed_mul(fixed_point value_a, fixed_point value_b)
{
    fixed_point result = 0;
    bool int_a = fixed_frac(value_a) == 0;
    bool int_b = fixed_frac(value_b) == 0;

    if (int_a && int_b)
    {
        result = fixed_itox(fixed_xtoi(value_a) * fixed_xtoi(value_b));
    }
    else
    {
        if (fixed_frac(value_a) == 0) result = fixed_xtoi(value_a) * value_b;
        else if (fixed_frac(value_b) == 0) result = value_a * fixed_xtoi(value_b);
        else result = (fixed_point)(((large_int)value_a * (large_int)value_b) >> FIXED_FRACTION);
    }
    return result;
}

fixed_point fixed_div(fixed_point value_a, fixed_point value_b)
{
    fixed_point result = 0;

    if (value_b != 0)
    {
        if (fixed_frac(value_b) == 0) result = value_a / fixed_xtoi(value_b);
        else result = (fixed_point)(((large_int)value_a << FIXED_FRACTION) / value_b);
    }
    else
    {
        result = fixed_mul(fixed_sign(value_a), fixed_inf);
    }

    return result;
}

fixed_point fixed_frac(fixed_point value)
{
    return value & fixed_fractional_mask;
}

fixed_point fixed_floor(fixed_point value)
{
    return value & fixed_integer_mask;
}

fixed_point fixed_ceil(fixed_point value)
{
    fixed_point result = value & fixed_integer_mask;
    if (result > 0 && fixed_frac(value) != 0)
    {
        result += 1;
    }

    return result;
}

fixed_point fixed_round(fixed_point value)
{
    fixed_point half = fixed_div(fixed_one, fixed_one << 1);

    return fixed_floor(value + half);
}

fixed_point fixed_abs(fixed_point value)
{
    return value >= 0 ? value : -value;
}

fixed_point fixed_idiv(fixed_point value_a, fixed_point value_b)
{
    return fixed_floor(fixed_div(value_a, value_b));
}

fixed_point fixed_mod(fixed_point value_a, fixed_point value_b)
{
    fixed_point q = fixed_div(value_a, value_b);
    return value_a - fixed_mul(fixed_floor(q), value_b);
}

fixed_point fixed_unm(fixed_point value)
{
    return -value;
}


fixed_point fixed_pow_int(fixed_point base, fixed_point exp)
{
    fixed_point result = 0;

    if (exp < 0)
    {
        result = fixed_div(fixed_one, fixed_pow_int(base, -exp));
    }
    else
    {
        result = fixed_one;

        while (exp > 0)
        {
            if (exp & 1)
            {
                result = fixed_mul(result, base);
            }
            exp >>= 1;
            base = fixed_mul(base, base);
            if (base == 0 || result == 0)
            {
                return 0;
            }
        }
    }

    return result;
}

fixed_point fixed_pow_real(fixed_point value_a, fixed_point value_b)
{
    /*
        a ^ x = (e ^ (ln a)) ^ x
              = e ^ (x * ln a)
    */
    return fixed_exp(fixed_mul(value_b, fixed_log(value_a)));
}

fixed_point fixed_pow(fixed_point value_a, fixed_point value_b)
{
    fixed_point result = 0;

    if (value_b == 0)
    {
        result = fixed_one;
    }
    else
    {
        fixed_point fval = fixed_frac(value_b);
        if (fval > 0 && value_a < 0)
        {
            result = fixed_nan;
        }
        else
        {
            result = fixed_pow_int(value_a, fixed_xtoi(value_b));
            if (fval > 0)
            {
                result = fixed_mul(result, fixed_pow_real(value_a, fval));
            }
        }
    }

    return result;
}

fixed_point fixed_lerp(fixed_point (&values)[0x400], fixed_point value, fixed_point end_value)
{
    if (value >= fixed_one) return end_value;
    else if (value <= 0) return values[0];
    else
    {
        fixed_point pos = value << 10;
        int index = (int)fixed_xtoi(pos);
        fixed_point value_a = values[index];
        fixed_point value_b = index < 0x400 - 1 ? values[index + 1] : end_value;
        return value_a + fixed_mul(value_b - value_a, fixed_frac(pos));
    }
}

fixed_point fixed_exp_pow(fixed_point value)
{
    // look up
    return fixed_lerp(fixed_lut_exp, value, fixed_e);

    //// While the value is larger than epsilon, shrink the value by half...
    //if (value > fixed_epsilon)
    //{
    //    // e ^ x = (e ^ (x / 2)) ^ 2
    //    fixed_point ee = fixed_exp_pow(value / 2);
    //    return fixed_mul(ee, ee);
    //}
    //// When the value is smaller enough
    //else
    //{
    //    //// Taylor's formula: e ^ x ~= 1 + x + (x ^ 2) / 2! + (x ^ 3) / 3! + (x ^ 4) / 4! ...
    //    //return fixed_one + value + fixed_pow_int(value, 2) / 2 + fixed_pow_int(value, 3) / 6 + fixed_pow_int(value, 4) / 24;
    //}
}

fixed_point fixed_exp(fixed_point value)
{
    if (value == 0)
    {
        return fixed_one;
    }
    else if (value < 0)
    {
        // e ^ -x = 1 / (e ^ x)
        return fixed_div(fixed_one, fixed_exp(-value));
    }
    else
    {
        // e ^ (a + b) = (e ^ a) * (e ^ b) => e ^ x = (e ^ int(x)) * (e ^ frac(x))
        return fixed_mul(fixed_pow_int(fixed_e, fixed_xtoi(value)), fixed_exp_pow(fixed_frac(value)));
    }
}

fixed_point fixed_log(fixed_point value)
{
    /*
        ln(x) = ln(a * 2 ^ n)
              = ln(a) + n * ln(2)
    */
    if (value > 0)
    {
        if (value == fixed_one) return 0;
        else
        {
            int pot = 0;

            while (value >= fixed_two)
            {
                value = value >> 1;
                pot++;
            }

            while (value < fixed_one)
            {
                value = value << 1;
                pot--;
            }

            return fixed_lerp(fixed_lut_ln, value - fixed_one, fixed_log_two) + pot * fixed_log_two;
        }
    }
    else
    {
        return fixed_nan;
    }
}

fixed_point fixed_sqrt(fixed_point value)
{
    return fixed_pow(value, fixed_half);
    //static fixed_point fixed_hundred = fixed_one * 100;
    //if (value >= fixed_hundred) return 10 * fixed_sqrt(value / 100);
    //else
    //{
    //    fixed_point iter = fixed_div(value, fixed_two);
    //    int count = 10;
    //    while (count-- > 0)
    //    {
    //        if (fixed_abs(value - fixed_mul(iter, iter)) <= fixed_epsilon) break;
    //        iter = fixed_div(fixed_div(value, iter) + iter, fixed_two);
    //    }
    //    return iter;
    //}
}

fixed_point fixed_sin(fixed_point value)
{
    return fixed_cos((fixed_half_pi) - value);
}

fixed_point fixed_cos(fixed_point value)
{
    if (value < 0) value = -value;
    
    static fixed_point fixed_double_pi = fixed_pi << 1;
    value = fixed_mod(value, fixed_double_pi);
    if (value > fixed_pi) value = fixed_double_pi - value;
    return fixed_lerp(fixed_lut_cos, fixed_div(value, fixed_pi), -fixed_one);
}

fixed_point fixed_tan(fixed_point value)
{
    return fixed_div(fixed_sin(value), fixed_cos(value));
}

fixed_point fixed_asin(fixed_point value)
{
    if (fixed_abs(value) > fixed_one)
    {
        return fixed_nan;
    }
    else
    {
        return fixed_lerp(fixed_lut_asin, (value + fixed_one) >> 1, fixed_half_pi);
    }
}

fixed_point fixed_acos(fixed_point value)
{
    fixed_point result = fixed_asin(value);
    return fixed_isnan(result) ? result : fixed_half_pi - result;
}

fixed_point fixed_atan(fixed_point value)
{
    if (fixed_abs(value) > fixed_one)
    {
        return (value > 0 ? fixed_half_pi : -fixed_half_pi) - fixed_atan(fixed_div(fixed_one, value));
    }
    else
    {
        return fixed_lerp(fixed_lut_atan, (value + fixed_one) >> 1, fixed_half_pi >> 1);
    }
}

fixed_point fixed_atan2(fixed_point value_a, fixed_point value_b)
{
    if (value_b != 0)
    {
        return fixed_atan(fixed_div(value_a, value_b));
    }
    else
    {
        if (value_a > 0) return fixed_half_pi;
        else if (value_a < 0) return -fixed_half_pi;
        else return fixed_nan;
    }
}

fixed_point fixed_rad(fixed_point value)
{
    return fixed_mul(value, fixed_pi) / 180;
}

fixed_point fixed_deg(fixed_point value)
{
    return fixed_div(value, fixed_pi) * 180;
}
