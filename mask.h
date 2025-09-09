#ifndef mask_H
#define mask_H

#include "common.h"

/// @brief Checks if all bits in `rhs` are set in `lhs`.
/// @param lhs Source mask.
/// @param rhs Mask to check for.
/// @return true if all bits in `rhs` are present in `lhs`, false otherwise.
bool m8_has(mask8_t lhs, mask8_t rhs);

/// @brief Removes the bits in `rhs` from `src`.
/// @param src Source mask.
/// @param rhs Bits to remove.
/// @return The resulting mask after removal.
mask8_t m8_remove(mask8_t src, mask8_t rhs);

/// @brief Adds the bits in `rhs` to `src`.
/// @param src Source mask.
/// @param rhs Bits to add.
/// @return The resulting mask after addition.
mask16_t m8_add(mask8_t src, mask8_t rhs);

bool m16_has(mask16_t lhs, mask16_t rhs);
mask16_t m16_add(mask16_t src, mask16_t rhs);
mask16_t m16_remove(mask16_t src, mask16_t rhs);

bool m32_has(mask32_t lhs, mask32_t rhs);
mask32_t m32_remove(mask32_t src, mask32_t rhs);
mask16_t m32_add(mask32_t src, mask32_t rhs);

bool m64_has(mask64_t lhs, mask64_t rhs);
mask64_t m64_remove(mask64_t src, mask64_t rhs);
mask16_t m64_add(mask64_t src, mask64_t rhs);

/// @brief Extracts a segment of `bits_to_segment` bits from `rhs`.
/// @param bits_to_segment Number of bits to extract.
/// @param rhs Source mask.
/// @return The extracted bits as mask64_t.
mask64_t m8_get_segment(uint32_t bits_to_segment, mask8_t rhs);
mask64_t m16_get_segment(uint32_t bits_to_segment, mask16_t rhs);
mask64_t m32_get_segment(uint32_t bits_to_segment, mask32_t rhs);
mask64_t m64_get_segment(uint32_t bits_to_segment, mask64_t rhs);

#endif // mask_H
