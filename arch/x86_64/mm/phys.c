#include <arch/x86_64/mm/phys.h>
#include <arch/x86_64/boot/params.h>

#include <inix/defs.h>
#include <inix/mm/scratch.h>
#include <inix/mm/type.h>
#include <inix/mm/paging.h>

#include <stddef.h>

// Frame stack pointer
static ptr_t stack_top;
static const uint64_t PHYS_LOW_MARK = 0x100000;

// Boot parameters -- needed for the memory map
extern boot_params_t boot_params;

void arch_phys_init()
{
    for(uint64_t i = 0; i < boot_params.map.count; i++) {
        memory_map_entry_t* entry = &boot_params.map.entries[i];

        if(entry->flags != PHYS_NORMAL_MEMORY || entry->phys_addr < PHYS_LOW_MARK) {
            continue;
        }

        uint64_t pages = entry->num_bytes / VM_PAGE_SIZE;

        for(uint64_t p = 0; p < pages; p++) {
            uint64_t* frame = scratch_map(entry->phys_addr + p * VM_PAGE_SIZE);

            if(p == pages - 1) {
                if(i == boot_params.map.count - 1) {
                    *frame = 0;
                } else {
                    uint64_t pos = i + 1;

                    while(pos < boot_params.map.count && boot_params.map.entries[pos].flags != PHYS_NORMAL_MEMORY)
                        pos++;

                    if(pos == boot_params.map.count) {
                        *frame = 0;
                    } else {
                        *frame = boot_params.map.entries[pos].phys_addr;
                    }
                }
            } else {
                *frame = entry->phys_addr + (p + 1) * VM_PAGE_SIZE;
            }

            scratch_unmap(frame);
        }

        // Setup top of the stack
        if(stack_top == 0) {
            stack_top = entry->phys_addr;
        }
    }
}

ptr_t arch_phys_allocate_frame()
{
    // Out of memory!
    if(stack_top == 0) {
        return 0;
    }

    uint64_t* next = scratch_map(stack_top);

    ptr_t frame = stack_top;
    stack_top = *next;

    // Zero out the frame -- TODO replace with memset call
    // TODO -- make this step optional by taking a flag
    for(uint64_t i = 0; i < VM_PAGE_SIZE / 8; i++)
        *next = 0;

    scratch_unmap(next);
    return frame;
}

void arch_phys_free_frame(ptr_t frame)
{
    uint64_t* next = scratch_map(frame);

    *next = stack_top;
    stack_top = frame;

    scratch_unmap(next);
}
