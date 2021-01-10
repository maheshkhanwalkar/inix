#include <stdint.h>
#include <arch/x86_64/mm/pml4.h>
#include <arch/x86_64/mm/phys.h>
#include <arch/x86_64/mm/paging.h>
#include <arch/x86_64/mm/invlpg.h>

#include <inix/mm/phys.h>
#include <inix/mm/scratch.h>

// Base page tables
uint64_t pml4e[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t pdpt[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t pd[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t pt[PT_NUM_ENTRIES][PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));

// Scratch space tables
uint64_t scratch_pd[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t scratch_pt[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));

// Stack page table
uint64_t stack_pt[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));

static uint64_t* get_table(uint64_t* table, unsigned int pos)
{
    uint64_t raw = table[pos];
    uint64_t addr = raw & (~(PAGE_SIZE - 1));

    // Allocate the table, if needed
    if(addr == 0) {
        addr = phys_allocate_frame(ARQ_ATOMIC);

        if(addr == 0) {
            panic("out of memory");
        }

        table[pos] = addr | PG_PRESENT | PG_WRITE | PG_NO_EXECUTE;
    }

    return (uint64_t*)scratch_map(addr);
}

void pml4_map_page(uint64_t v_addr, uint64_t phys_addr, unsigned long flags)
{
    unsigned int pml4_pos = PML4_INDEX(v_addr);
    unsigned int pdpt_pos = PDPT_INDEX(v_addr);
    unsigned int pd_pos   = PD_INDEX(v_addr);
    unsigned int pt_pos   = PT_INDEX(v_addr);

    uint64_t* pdpt_table = get_table(pml4e, pml4_pos);
    uint64_t* pd_table = get_table(pdpt_table, pdpt_pos);
    uint64_t* pt_table = get_table(pd_table, pd_pos);

    pt_table[pt_pos] = phys_addr | flags;
    _invlpg(v_addr);

    // Unmap the tables
    scratch_unmap(pdpt_table);
    scratch_unmap(pd_table);
    scratch_unmap(pt_table);
}
