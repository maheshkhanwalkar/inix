#include <arch/x86_64/boot/params.h>
#include <arch/x86_64/mm/paging.h>
#include <arch/x86_64/mm/pml4.h>
#include <arch/x86_64/mm/invlpg.h>

#include <inix/mm/vm.h>
#include <stdint.h>

#include <inix/mm/paging.h>

static uint64_t virt_base = 0xFFFF800000000000;
extern boot_params_t boot_params;

uint64_t arch_vm_carve(uint64_t pages)
{
    // FIXME: this needs to be more complicated than this, but for a first revision
    //  it's okay -- but needs to be more robust

    if(virt_base == boot_params.kernel_start)
        return 0;

    if(virt_base + pages * VM_PAGE_SIZE >= boot_params.kernel_start)
        return 0;

    uint64_t addr = virt_base;
    virt_base += pages * VM_PAGE_SIZE;

    return addr;
}

void arch_vm_map_page(uint64_t v_addr, uint64_t phys_addr, uint32_t flags)
{
    unsigned long arch_flags = 0;

    if(flags & VM_PG_PRESENT)
        arch_flags |= PG_PRESENT;
    if(flags & VM_PG_WRITE)
        arch_flags |= PG_WRITE;
    if(!(flags & VM_PG_EXECUTE))
        arch_flags |= PG_NO_EXECUTE;

    pml4_map_page(v_addr, phys_addr, arch_flags);
}

void arch_vm_invlpg(uint64_t v_addr)
{
    _invlpg(v_addr);
}

uint64_t arch_vm_translate(uint64_t v_addr)
{
    return pml4_translate(v_addr);
}
