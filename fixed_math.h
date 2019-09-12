#pragma once

#define FIXED_FRACTION 32

#ifdef FIXED_VALUE_TYPE
typedef FIXED_VALUE_TYPE fixed_point;
#else
typedef long long fixed_point;
#endif

#ifdef __cpp_constexpr
#define CONSTEXPR constexpr
#else
#define CONSTEXPR inline
#endif

#define fixed_integer_mask      (fixed_point)0xFFFFFFFF00000000
#define fixed_fractional_mask   (fixed_point)0x00000000FFFFFFFF
#define fixed_one               (fixed_point)0x0000000100000000
#define fixed_two               (fixed_point)0x0000000200000000
#define fixed_half              (fixed_point)0x0000000080000000
#define fixed_max               (fixed_point)0x7FFFFFFFFFFFFFFD
#define fixed_inf               (fixed_point)0x7FFFFFFFFFFFFFFF
#define fixed_nan               (fixed_point)0xFFFFFFFFFFFFFFFE
#define fixed_pi                (fixed_point)0x00000003243F6A88 /* 3.14159265 */
#define fixed_half_pi           (fixed_point)0x00000001921FB544 /* 1.57079633 */
#define fixed_e                 (fixed_point)0x00000002B7E15162 /* 2.71828183 */
#define fixed_log_two           (fixed_point)0x00000000B1721886 /* 0.69314718 */
#define fixed_epsilon           (fixed_point)0x00000000000010C6 /* 0.00000100 */

#ifdef __cplusplus
extern "C"
{
#endif
    fixed_point fixed_sign(fixed_point value);
    int fixed_isinf(fixed_point value);
    int fixed_isnan(fixed_point value);

    fixed_point fixed_add(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_sub(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_mul(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_div(fixed_point value_a, fixed_point value_b);

    fixed_point fixed_frac(fixed_point value);
    fixed_point fixed_floor(fixed_point value);
    fixed_point fixed_ceil(fixed_point value);
    fixed_point fixed_round(fixed_point value);
    fixed_point fixed_abs(fixed_point value);
    fixed_point fixed_idiv(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_mod(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_unm(fixed_point value);

    fixed_point fixed_pow(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_sqrt(fixed_point value);
    fixed_point fixed_exp(fixed_point value);
    fixed_point fixed_log(fixed_point value);

    fixed_point fixed_sin(fixed_point value);
    fixed_point fixed_cos(fixed_point value);
    fixed_point fixed_tan(fixed_point value);
    fixed_point fixed_asin(fixed_point value);
    fixed_point fixed_acos(fixed_point value);
    fixed_point fixed_atan(fixed_point value);
    fixed_point fixed_atan2(fixed_point value_a, fixed_point value_b);
    fixed_point fixed_rad(fixed_point value);
    fixed_point fixed_deg(fixed_point value);

    CONSTEXPR fixed_point fixed_itox(int value)
    {
        return (fixed_point)value << FIXED_FRACTION;
    }

    CONSTEXPR fixed_point fixed_natox(const char* s, size_t len)
    {
        fixed_point result = 0;
        fixed_point ival = 0;
        fixed_point fval = 0;
        fixed_point exp = 1;
        int neg = 0;
        const char* e = len == 0 ? 0 : s + len;

        while ((e == 0 || s <= e) && (*s == ' ' || *s == '\t')) s++;  /* skip initial spaces */

        if (*s == '-')
        {
            s++;
            neg = 1;
        }

        for (; (e == 0 || s <= e) && (*s >= '0' && *s <= '9'); s++) {
            int d = *s - '0';
            ival = ival * 10 + d;
        }

        if (*s == '.')
        {
            s++;
            for (; (e == 0 || s <= e) && (*s >= '0' && *s <= '9'); s++) {
                int d = *s - '0';
                fval = fval * 10 + d;
                exp = exp * 10;
            }
        }

        result = (ival << FIXED_FRACTION) + (fval << FIXED_FRACTION) / exp;

        if (neg) result = -result;

        return result;
    }

    CONSTEXPR fixed_point fixed_atox(const char* s)
    {
        return fixed_natox(s, 0);
    }

    CONSTEXPR int fixed_xtoi(fixed_point value)
    {
        return value >> FIXED_FRACTION;
    }

    CONSTEXPR double fixed_xtod(fixed_point value)
    {
        return (double)value / fixed_one;
    }

#ifdef __cplusplus
}
#endif