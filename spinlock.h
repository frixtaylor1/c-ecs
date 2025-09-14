#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdatomic.h>

typedef struct {
    volatile int lock;
} Spinlock;

static inline void spinlock_init(Spinlock* s) {
    s->lock = 0;
}

static inline void spinlock_acquire(Spinlock* s) {
    /*****************************/
    /*      busy wait (spin)     */
    /*****************************/
    while (__sync_lock_test_and_set(&s->lock, 1));
}

static inline void spinlock_release(Spinlock* s) {
    __sync_lock_release(&s->lock);
}

#endif /* SPINLOCK_H */