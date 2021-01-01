#include <mm/vm.h>

extern uintptr_t vm_arch_carve(uintptr_t pages);
extern void vm_arch_map_page(uintptr_t v_addr, uintptr_t phys_addr, uint32_t flags);

uintptr_t vm_carve(uintptr_t pages)
{
    return vm_arch_carve(pages);
}

void vm_map_page(uintptr_t v_addr, uintptr_t phys_addr, uint32_t flags)
{
    vm_arch_map_page(v_addr, phys_addr, flags);
}
