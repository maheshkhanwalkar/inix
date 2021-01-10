#pragma once

/**
 * Allocation request type.
 *
 * The kernel supports two kinds of memory allocation requests:
 *
 *   ARQ_ATOMIC - allocate the memory if possible, fail otherwise.
 *
 *     With the ARQ_ATOMIC flag, the allocation is best-effort and will fail
 *     when the system does not have enough available memory
 *
 *   ARQ_SLEEP  - allocate memory, waiting until enough memory is available
 *
 *     With the ARQ_SLEEP flag, the allocation request will always succeed,
 *     but may take time -- the kernel or user context will be put to sleep
 *     if the request cannot be immediately fulfilled.
 */
typedef enum alloc_req {
    ARQ_ATOMIC,
    ARQ_SLEEP
} alloc_req_t;
