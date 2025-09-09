#ifndef alloc_H
#define alloc_H

#include "common.h"

/// @brief Simple pool allocator structure for managing memory blocks.
typedef struct PoolAllocator {
    heap_t arena;
    uint32_t capacity;
} PoolAllocator;

/// @brief Initializes a memory pool with a given capacity.
/// @param pa Pool allocator instance
/// @param capacity Maximum size of the pool in bytes
$public void m_initialize(PoolAllocator* pa, uint32_t capacity);

/// @brief Destroys the memory pool and frees its resources.
/// @param pa Pool allocator instance
$public void m_destroy(PoolAllocator* pa);

/// @brief Allocates a memory block from the pool.
/// @param pa Pool allocator instance
/// @param byte_ts Size of the memory block in bytes
/// @return Pointer to the allocated memory, or NULL on failure
$public void* m_alloc(PoolAllocator* pa, uint32_t byte_ts);

/// @brief Deallocates a memory block previously allocated.
/// @param ptr Pointer to the memory block
$public void m_dealloc(void* ptr);

/// @brief Reallocates a memory block to a new size.
/// @param pa Pool allocator instance
/// @param ptr Pointer to the existing memory block
/// @param byte_ts New size in bytes
/// @return Pointer to the reallocated memory block, or NULL on failure
$public void* m_realloc(PoolAllocator* pa, void* ptr, uint32_t byte_ts);

#endif // alloc_H
