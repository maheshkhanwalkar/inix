#include <mm/scratch.h>

// Arch-specific references
extern void* arch_scratch_map(ptr_t phys_addr);
extern void arch_scratch_unmap(void* virt_addr);

void* scratch_map(ptr_t phys_addr)
{
    return arch_scratch_map(phys_addr);
}

void scratch_unmap(void* virt_addr)
{
    arch_scratch_unmap(virt_addr);
}
