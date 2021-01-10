#include <stdint.h>
#include <stddef.h>

#include <arch/x86_64/mm/pml4.h>
#include <arch/x86_64/mm/paging.h>
#include <arch/x86_64/mm/invlpg.h>

#include <inix/mm/type.h>

static const uint64_t SCRATCH_BASE_ADDR = 0xFFFFFFFFC0000000;
extern uint64_t scratch_pt[PT_NUM_ENTRIES];

void* arch_scratch_map(uint64_t frame, alloc_req_t type)
{
    // Ignore type for now -- we only support ARQ_ATOMIC
    (void)type;

    for(int i = 0; i < PT_NUM_ENTRIES; i++) {
        if(scratch_pt[i] == 0) {
            scratch_pt[i] = frame | PG_PRESENT | PG_WRITE | PG_NO_EXECUTE;

            uint64_t virt = SCRATCH_BASE_ADDR + i * PAGE_SIZE;
            _invlpg(virt);

            return (void*)virt;
        }
    }

    // Out of scratch space!
    return NULL;
}

void arch_scratch_unmap(void* scratch)
{
    uint64_t equiv = (uint64_t)scratch;

    if(equiv < SCRATCH_BASE_ADDR)
        return;

    uint64_t pos = (equiv - SCRATCH_BASE_ADDR) / PAGE_SIZE;

    if(pos >= PT_NUM_ENTRIES)
        return;

    scratch_pt[pos] = 0;
    _invlpg(equiv);
}
