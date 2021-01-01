#include <arch/x86_64/mm/phys.h>
#include <arch/x86_64/boot/params.h>

#include <stdint.h>
#include <stdbool.h>

#define PHYS_BLOCK_GRANULARITY 0x1000
#define PHYS_LOW_MEMORY_BAR 0x100000

extern boot_params_t boot_params;

static bool check_condition(unsigned long phys_addr, phys_zone_t zone)
{
    if(zone == PHYS_ZONE_NORMAL && phys_addr >= PHYS_LOW_MEMORY_BAR)
        return true;

    if(zone == PHYS_ZONE_LOW_MEMORY && phys_addr < PHYS_LOW_MEMORY_BAR)
        return true;

    return false;
}

/**
 * Carve out a contiguous chunk of memory from the specified memory zone
 *
 * The specified amount should be a multiple of the physical memory
 * subsystem block granularity (PHYS_BLOCK_GRANULARITY), which is silently
 * enforced, if that is not the case.
 *
 * This function simply scans the memory map and finds the first section
 * that can accommodate the request and returns the memory. If no such section
 * exists, then it simply returns 0 as an error.
 *
 * This allocator does not support giving back physical memory -- all memory
 * management is delegated up to the virtual memory manager which handles
 * allocation, freeing, and coalescing of fine-grained blocks of memory.
 */
uint64_t phys_carve(uint64_t amt, phys_zone_t zone)
{
    // Ensure we are allocating at page-granularity
    amt = (amt + PHYS_BLOCK_GRANULARITY - 1) / PHYS_BLOCK_GRANULARITY * PHYS_BLOCK_GRANULARITY;

    for(unsigned long i = 0; i < boot_params.map.count; i++) {
        memory_map_entry_t* entry = &boot_params.map.entries[i];

        if(entry->num_bytes >= amt && check_condition(entry->phys_addr, zone)) {
            uint64_t address = entry->phys_addr;

            entry->phys_addr += amt;
            entry->num_bytes -= amt;

            return address;
        }
    }

    // Could not allocate!
    return 0;
}
