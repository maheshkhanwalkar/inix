#include <mm/phys.h>
#include <mm/vm.h>

void kernel_main()
{
    uintptr_t phys = phys_carve(0x1000, 0);
    uintptr_t virt = vm_carve(1);

    vm_map_page(virt, phys, VM_PG_PRESENT | VM_PG_WRITE);
}
