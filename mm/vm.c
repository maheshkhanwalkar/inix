#include <mm/vm.h>

// Arch-specific references
extern uintptr_t vm_arch_carve(uintptr_t pages);
extern void vm_arch_map_page(uintptr_t v_addr, uintptr_t phys_addr, uint32_t flags);
extern void vm_arch_invlpg(uintptr_t v_addr);
extern uint32_t vm_arch_page_size();

uintptr_t vm_carve(uintptr_t pages)
{
    return vm_arch_carve(pages);
}

void vm_map_page(uintptr_t v_addr, uintptr_t phys_addr, uint32_t flags)
{
    vm_arch_map_page(v_addr, phys_addr, flags);
}

void vm_invlpg(uintptr_t v_addr)
{
    vm_arch_invlpg(v_addr);
}

uint32_t vm_page_size()
{
    return vm_arch_page_size();
}
