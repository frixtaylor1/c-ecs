#ifndef common_H
#define common_H

#include <stdint.h>

typedef uint8_t mask8_t;
typedef uint16_t mask16_t;
typedef uint32_t mask32_t;
typedef uint64_t mask64_t;
typedef uint32_t word_t;
typedef char byte_t;
typedef void* heap_t;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define $packed __attribute__((packed))
#define $public __attribute__((visibility("default")))
#define $private static

typedef void (*defer_func)(void*);

typedef struct Defer {
    defer_func func;
    void* arg;
} Defer;

static inline void
run_defer(Defer* d)
{
    if (d->func)
        d->func(d->arg);
}

// macro auxiliar
#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)

#define defer(func, args...) \
    __attribute__((cleanup(run_defer))) Defer CONCAT(_defer_, __LINE__) = { (defer_func)(func), (void*)(args) }

#define bool uint32_t

#endif // common_H