#include <inix/mm/vm.h>

// Arch-specific references
extern ptr_t arch_vm_carve(ptr_t pages);
extern void arch_vm_map_page(ptr_t v_addr, ptr_t phys_addr, uint32_t flags);
extern void arch_vm_invlpg(ptr_t v_addr);
extern ptr_t arch_vm_translate(ptr_t v_addr);

ptr_t vm_carve(ptr_t pages)
{
    return arch_vm_carve(pages);
}

void vm_map_page(ptr_t v_addr, ptr_t phys_addr, uint32_t flags)
{
    arch_vm_map_page(v_addr, phys_addr, flags);
}

void vm_invlpg(ptr_t v_addr)
{
    arch_vm_invlpg(v_addr);
}

ptr_t vm_translate(ptr_t v_addr)
{
    return arch_vm_translate(v_addr);
}
