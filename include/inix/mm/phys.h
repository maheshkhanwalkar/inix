#pragma once
#include <inix/defs.h>
#include <inix/mm/type.h>

/**
 * Allocate a physical frame.
 *
 * This function returns a physical frame of size PAGE_SIZE,
 * which can then be mapped into the virtual address space.
 */
ptr_t phys_allocate_frame(void);

/**
 * Free a physical frame
 *
 * This function returns the physical frame back to the frame
 * allocator so that it can be reused by a future call to
 * phys_allocate_frame(...)
 */
void phys_free_frame(ptr_t frame);
