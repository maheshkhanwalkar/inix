#include <inix/mm/phys.h>

// Arch-specific calls
extern ptr_t arch_phys_allocate_frame(void);
extern void arch_phys_free_frame(ptr_t frame);

ptr_t phys_allocate_frame(void)
{
    return arch_phys_allocate_frame();
}

void phys_free_frame(ptr_t frame)
{
    arch_phys_free_frame(frame);
}
