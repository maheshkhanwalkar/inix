#include <stdint.h>

extern uintptr_t phys_mem_carve(uintptr_t amt, uint32_t flags);

/**
 * Carve out a contiguous chunk of physical memory
 *
 * This function calls the underlying arch-specific phys_mem_carve()
 * to perform the actual allocation.
 *
 * The arch_flags parameter allows for arch-specific flags to be passed
 * to the underlying allocator -- so the entire kernel subsystem can
 * use the arch-independent allocator, rather than hooking into the
 * arch-specific one for *special* allocation requests.
 */
uintptr_t phys_carve(uintptr_t amt, uint32_t arch_flags)
{
    return phys_mem_carve(amt, arch_flags);
}
