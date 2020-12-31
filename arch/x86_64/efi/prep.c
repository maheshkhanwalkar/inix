#include <stdint.h>
#include <inix/elf.h>
#include <arch/x86_64/efi/memory.h>
#include <arch/x86_64/mm/pml4.h>

#define STACK_NUM_PAGES 4

typedef struct kern_img {
    unsigned long ph_count;
    elf64_pheader_t* ph_entries;
} kern_img_t;

typedef struct mem_map {
    unsigned long count;
    efi_memory_entry_t* map;
} mem_map_t;

typedef struct boot_params {
    kern_img_t* img;
    mem_map_t* map;
} boot_params_t;

/**
 * Boot parameters
 */
boot_params_t boot_params;

// Page table references (see arch/x86_64/mm/pml4.c)
extern uint64_t pml4e[PT_NUM_ENTRIES];
extern uint64_t pdpt[PT_NUM_ENTRIES];
extern uint64_t pd[PT_NUM_ENTRIES];
extern uint64_t pt[PT_NUM_ENTRIES][PT_NUM_ENTRIES];
extern uint64_t scratch_pd[PT_NUM_ENTRIES];
extern uint64_t scratch_pt[PT_NUM_ENTRIES];
extern uint64_t stack_pt[PT_NUM_ENTRIES];

/**
 * Perform necessary prep to establish a kernel baseline
 *
 * When the UEFI loader jumps into the kernel, it leaves has loaded the image in
 * an effectively random location. While the actual *physical* location does not really
 * matter, the virtual location does.
 *
 * The state is currently identity mapping, which is not ideal. This function sets up
 * the kernel's own page tables and moves itself to the higher half. The actual magic
 * of fixing the instruction pointer is done back in boot/boot.S
 */
void* low_level_prep(unsigned long kern_base, kern_img_t* kern_img, mem_map_t* mem_map, const uint64_t* prev_pml4)
{
    // Save parameters
    // FIXME: these addresses have not been adjusted to higher-half!
    boot_params.img = kern_img;
    boot_params.map = mem_map;

    unsigned long page_pos = 0, pt_pos = 0;

    // FIXME: we shouldn't be doing raw page table manipulation here, since it prevents being
    //  able to support different hierarchies (e.g. 5-level paging) and different page sizes
    // Setup the page tables, as needed
    for(unsigned long i = 0; i < kern_img->ph_count; i++) {
        elf64_pheader_t* hdr = &kern_img->ph_entries[i];

        unsigned long pages = (hdr->p_memsz + PAGE_SIZE - 1) / PAGE_SIZE;

        for(unsigned long p = 0; p < pages; p++) {
            // Handle wrap around
            if(page_pos == PT_NUM_ENTRIES) {
                page_pos = 0;
                pt_pos++;
            }

            unsigned long address = kern_base + pt_pos * PT_NUM_ENTRIES + page_pos * PAGE_SIZE;

            // Explicitly disallow W+X permissions
            unsigned long nx = (hdr->p_flags & PF_X) ? 0 : 1;
            unsigned long wr = (hdr->p_flags & PF_W) && nx ? 1 : 0;

            // Setup entry: address, write enable (?), no execute (?), present
            pt[pt_pos][page_pos] = address | (wr << 1) | (nx << 63) | 0x1;
            page_pos++;
        }
    }

    // Setup higher-level paging structures
    // Assumption: kernel vma is hard-coded at 0xFFFFFFFF80000000 -- FIXME!
    for(unsigned long i = 0; i < PT_NUM_ENTRIES; i++) {
        pd[i] = (unsigned long)&pt[i] | 0x3;
    }

    pdpt[PT_NUM_ENTRIES - 2] = (unsigned long)&pd | 0x3;
    pml4e[PT_NUM_ENTRIES - 1] = (unsigned long)&pdpt | 0x3;

    // Setup scratch space tables
    scratch_pd[0] = (unsigned long)&scratch_pt | 0x3;
    pdpt[PT_NUM_ENTRIES - 1] = (unsigned long)&scratch_pd | 0x3;

    // Setup stack page table (16k stack)
    for(unsigned long i = 0; i < mem_map->count; i++) {
        efi_memory_entry_t* entry = &mem_map->map[i];

        if(entry->type == EFI_CONVENTIONAL_MEMORY && entry->num_pages > 4) {
            for(int j = 0; j < STACK_NUM_PAGES; j++) {
                // P+W+NX
                stack_pt[(PT_NUM_ENTRIES - STACK_NUM_PAGES) + j] = (entry->phys_start + PAGE_SIZE * j) | (1UL << 63) | 0x3;
            }

            // PAGE_SIZE >= EFI_PAGE_SIZE is a guarantee
            entry->phys_start += STACK_NUM_PAGES * PAGE_SIZE;
            entry->num_pages -= STACK_NUM_PAGES * (PAGE_SIZE / EFI_PAGE_SIZE);

            break;
        }
    }

    scratch_pd[PT_NUM_ENTRIES - 1] = (unsigned long)&stack_pt | 0x3;

    // Setup current address space mapping -- this is just temporary, but needed
    // otherwise we'll get an immediate page-fault when %cr3 is modified.
    pml4e[0] = prev_pml4[0];
    return &pml4e;
}
