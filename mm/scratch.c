#include <inix/mm/scratch.h>
#include <inix/mm/type.h>

// Arch-specific references
extern void* arch_scratch_map(ptr_t frame, alloc_req_t type);
extern void arch_scratch_unmap(void* scratch);

void* scratch_map(ptr_t frame, alloc_req_t type)
{
    return arch_scratch_map(frame, type);
}

void scratch_unmap(void* scratch)
{
    arch_scratch_unmap(scratch);
}
