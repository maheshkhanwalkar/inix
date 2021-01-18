#pragma once
#include <inix/defs.h>

void vm_init(void);

/**
 * Allocate from the kernel heap the requested number of bytes
 *
 * The request is guaranteed (if it succeeds) to return a chunk of memory
 * that is at least 'amt' bytes long. In certain cases, it may return more
 * due to internal implementation logic, but that should not be assumed.
 */
void* vm_alloc(ptr_t amt);

/**
 * Free the memory, returning it back to the kernel heap
 *
 * This memory *must* have originated from the heap in the first place, otherwise
 * the free request will be ignored.
 */
void vm_free(void* ptr);

/**
 * Donate memory to the allocator
 *
 * This function should be used by kernel subsystems that have allocated memory
 * through direct means and would like to donate it to the VM allocator, so that it
 * may be reused by other kernel subsystems.
 */
void vm_donate(void* ptr, ptr_t amt);
