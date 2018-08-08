#pragma once
#include <stdint.h>

typedef uint64_t spinlock_t[8]__attribute__((aligned(64)));

/**
 * Acquire the spin-lock
 *
 * If interrupt-safety is required, the call to spinlock_acquire()
 * should be preceded by a call to disable_interrupts()
 *
 * @param lock - holding container
 */
void spinlock_acquire(spinlock_t lock);


/**
 * Release the spin-lock
 * @param lock - holding container
 */
void spinlock_release(spinlock_t lock);
