#include "alloc.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

typedef struct __attribute__((packed)) Header {
    uint32_t words : 30;
    bool alloced : 1;
    bool reserved : 1;
} Header;

#define $header (Header*)
#define $void (void*)
#define $byte_t (char*)

$private word_t
calculate_words(uint32_t byte_ts)
{
    return (byte_ts % 4 == 0) ? byte_ts / 4 : byte_ts / 4 + 1;
}

$private uint32_t
calculate_byte_ts(word_t words)
{
    return words * 4;
}

$private Header*
get_header(void* ptr)
{
    return (Header*)ptr - 1;
}

$private Header*
next_header(Header* header)
{
    return $header($byte_t header + calculate_byte_ts(header->words)) + 1;
}

$private bool
header_found(Header* header, word_t requestedwords)
{
    return !header->alloced && !header->reserved && (header->words == 0 || header->words >= requestedwords);
}

$private Header*
find_block(const PoolAllocator* pa, Header* header, word_t requestedwords)
{
    char* arena_end = (char*)pa->arena + pa->capacity;

    if ((char*)header >= arena_end) {
        assert(0 && "Out of memory: no block found!");
        return NULL;
    }

    if (header_found(header, requestedwords)) {
        return header;
    }

    return find_block(pa, next_header(header), requestedwords);
}

$private void*
get_block_area(Header* header)
{
    return header + 1;
}

$private void
initialize_first_header(PoolAllocator* pa)
{
    Header* h = (Header*)pa->arena;
    uint32_t usable = pa->capacity - sizeof(Header);
    word_t words = calculate_words(usable);
    h->words = words;
    h->alloced = 0;
    h->reserved = 0;
}

$public void
m_initialize(PoolAllocator* pa, uint32_t capacity)
{
    pa->capacity = capacity;

#ifdef _WIN32
    pa->arena = VirtualAlloc(NULL, capacity,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
#else
    void* mem = mmap(NULL, capacity,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        pa->arena = 0;
        pa->capacity = 0;
        return;
    }
    pa->arena = mem;
#endif

    initialize_first_header(pa);
}

$public void
m_destroy(PoolAllocator* pa)
{
#ifdef _WIN32
    VirtualFree(pa->arena, 0, MEM_RELEASE);
#else
    if (pa->arena && pa->capacity > 0) {
        munmap(pa->arena, pa->capacity);
    }
#endif
}

$public void*
m_alloc(PoolAllocator* pa, uint32_t byte_ts)
{
    word_t requestedwords = calculate_words(byte_ts);
    Header* header = (Header*)pa->arena;

    Header* selected = find_block(pa, header, requestedwords);
    selected->alloced = true;
    selected->reserved = false;
    selected->words = requestedwords;

    return get_block_area(selected);
}

$public void
m_dealloc(void* ptr)
{
    Header* header = get_header(ptr);

    assert(header->alloced && "Double free detected!");

    header->alloced = false;
    memset(ptr, 0, calculate_byte_ts(header->words));
}

$public void*
m_realloc(PoolAllocator* pa, void* ptr, uint32_t byte_ts)
{
    void* new_ptr = m_alloc(pa, byte_ts);
    if (!new_ptr)
        return NULL;

    Header* old_h = get_header(ptr);
    uint32_t old_size = calculate_byte_ts(old_h->words);

    memcpy(new_ptr, ptr, old_size < byte_ts ? old_size : byte_ts);

    m_dealloc(ptr);

    return new_ptr;
}
