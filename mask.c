#include "mask.h"

bool m8_has(mask8_t lhs, mask8_t rhs)
{
    return (lhs & rhs) != 0;
}

bool m16_has(mask16_t lhs, mask16_t rhs)
{
    return (lhs & rhs) != 0;
}

bool m32_has(mask32_t lhs, mask32_t rhs)
{
    return (lhs & rhs) != 0;
}

bool m64_has(mask64_t lhs, mask64_t rhs)
{
    return (lhs & rhs) != 0;
}

mask8_t m8_remove(mask8_t src, mask8_t rhs)
{
    return src &= ~rhs;
}

mask16_t m16_remove(mask16_t src, mask16_t rhs)
{
    return src &= ~rhs;
}

mask32_t m32_remove(mask32_t src, mask32_t rhs)
{
    return src &= ~rhs;
}

mask64_t m64_remove(mask64_t src, mask64_t rhs)
{
    return src &= ~rhs;
}

mask16_t m8_add(mask8_t src, mask8_t rhs)
{
    return src |= rhs;
}

mask16_t m16_add(mask16_t src, mask16_t rhs)
{
    return src |= rhs;
}

mask16_t m32_add(mask32_t src, mask32_t rhs)
{
    return src |= rhs;
}

mask16_t m64_add(mask64_t src, mask64_t rhs)
{
    return src |= rhs;
}

mask64_t m8_get_segment(uint32_t bits_to_segment, mask8_t rhs)
{
    return rhs >> bits_to_segment & 0xF;
}

mask64_t m16_get_segment(uint32_t bits_to_segment, mask16_t rhs)
{
    return rhs >> bits_to_segment & 0xF;
}

mask64_t m32_get_segment(uint32_t bits_to_segment, mask32_t rhs)
{
    return rhs >> bits_to_segment & 0xF;
}

mask64_t m64_get_segment(uint32_t bits_to_segment, mask64_t rhs)
{
    return rhs >> bits_to_segment & 0xF;
}