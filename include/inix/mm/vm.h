#pragma once
#include <inix/defs.h>
#include <stdbool.h>

#define VM_PG_PRESENT 0x1
#define VM_PG_WRITE   0x2
#define VM_PG_EXECUTE 0x4
#define VM_PG_USER    0x8

/**
 * Carve out a chunk of contiguous virtual address space
 *
 * This function does not perform any changes to the actual address
 * space configuration from a paging standpoint, rather, it only
 * determines a viable space which can be mapped to physical frames.
 */
ptr_t vm_carve(ptr_t pages);

/**
 * Map the given virtual address to a physical address, with the provided
 * permission flags
 */
void vm_map_page(ptr_t v_addr, ptr_t phys_addr, uint32_t flags);

/**
 * Invalidate a TLB entry for the given virtual page
 */
void vm_invlpg(ptr_t v_addr);

/**
 * Translate a virtual address to a physical address
 */
ptr_t vm_translate(ptr_t v_addr);

/**
 * Unmap the given virtual address
 *
 * When the virtual address is not associated with anything, then the
 * call becomes a no-op.
 */
void vm_unmap_page(ptr_t v_addr);

/**
 * Check whether there exists a present page entry within the paging structure
 * for the given virtual address
 */
bool vm_has_mapping(ptr_t v_addr);
