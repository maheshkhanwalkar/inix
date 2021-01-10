#pragma once
#include <inix/defs.h>
#include <inix/mm/type.h>

/**
 * Map the given physical page frame within the scratch space,
 * returning the virtual mapping address.
 *
 * This method should not be used directly (for the most part), since
 * the vm_map_page(...) function is sufficient for most use-cases.
 */
void* scratch_map(ptr_t frame);

/**
 * Unmap the scratch space page, allowing the virtual address to be
 * returned in a future call to scratch_map(...)
 */
void scratch_unmap(void* scratch);
