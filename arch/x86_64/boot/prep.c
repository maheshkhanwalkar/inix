#include <stdint.h>

/*
 * FIXME: it's probably worth breaking this file into a few pieces
 *  since there's a lot of moving pieces here
 */

/* Section permissions */
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

typedef struct elf64_pheader {
    unsigned int p_type;
    unsigned int p_flags;
    unsigned long p_offset;
    unsigned long p_vaddr;
    unsigned long p_paddr;
    unsigned long p_filesz;
    unsigned long p_memsz;
    unsigned long p_align;
} elf64_pheader_t;

typedef struct kern_img {
    unsigned long count;
    elf64_pheader_t* entries;
} kern_img_t;

#define EFI_CONVENTIONAL_MEMORY 0x7

typedef struct efi_memory_entry {
    unsigned int type;
    unsigned long phys_start;
    unsigned long virt_start; /* don't care */
    unsigned long num_pages;
    unsigned long attribute;
    unsigned long padding;
} efi_memory_entry_t;

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

/* Base page tables */
uint64_t pml4e[512]__attribute__((aligned(0x1000)));
uint64_t pdpt[512]__attribute__((aligned(0x1000)));
uint64_t pd[512]__attribute__((aligned(0x1000)));
uint64_t pt[512][512]__attribute__((aligned(0x1000)));

/* Scratch space tables */
uint64_t scratch_pd[512]__attribute__((aligned(0x1000)));
uint64_t scratch_pt[512]__attribute__((aligned(0x1000)));

/* Stack page table */
uint64_t stack_pt[512]__attribute__((aligned(0x1000)));

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
    /* Save parameters */
    /* FIXME: these addresses have not been adjusted to higher-half! */
    boot_params.img = kern_img;
    boot_params.map = mem_map;

    unsigned long page_pos = 0, pt_pos = 0;

    // Setup the page tables, as needed
    // TODO: actually check the segments headers, rather than the program headers
    //  Sometimes the program sections contain *multiple* segments, so we end up getting
    //  the superset of permissions (e.g. if a load section contains text and data, it will
    //  be marked as R+W+X, which is *not* desired at all)
    for(unsigned long i = 0; i < kern_img->count; i++) {
        elf64_pheader_t* hdr = &kern_img->entries[i];

        unsigned long pages = (hdr->p_memsz + 0x1000 - 1) / 0x1000;

        for(unsigned long p = 0; p < pages; p++) {
            // Handle wrap around
            if(page_pos == 512) {
                page_pos = 0;
                pt_pos++;
            }

            unsigned long address = kern_base + pt_pos * 512 + page_pos * 0x1000;

            unsigned long wr = (hdr->p_flags & PF_W) ? 1 : 0;
            unsigned long nx = (hdr->p_flags & PF_X) ? 0 : 1;

            // Setup entry: address, write enable (?), no execute (?), present
            pt[pt_pos][page_pos] = address | (wr << 1) | (nx << 63) | 0x1;
            page_pos++;
        }
    }

    // Setup higher-level paging structures
    // Assumption: kernel vma is hard-coded at 0xFFFFFFFF80000000 -- FIXME!
    for(unsigned long i = 0; i < 512; i++) {
        pd[i] = (unsigned long)&pt[i] | 0x3;
    }

    pdpt[510] = (unsigned long)&pd | 0x3;
    pml4e[511] = (unsigned long)&pdpt | 0x3;

    // Setup scratch space tables
    scratch_pd[0] = (unsigned long)&scratch_pt | 0x3;
    pdpt[511] = (unsigned long)&scratch_pd | 0x3;

    // Setup stack page table (16k stack)
    for(unsigned long i = 0; i < mem_map->count; i++) {
        efi_memory_entry_t* entry = &mem_map->map[i];

        if(entry->type == EFI_CONVENTIONAL_MEMORY && entry->num_pages > 4) {
            for(int j = 0; j < 4; j++) {
                /* P+W+NX */
                stack_pt[(512 - 4) + j] = (entry->phys_start + 0x1000 * j) | (1UL << 63) | 0x3;
            }

            entry->phys_start += 4 * 0x1000;
            entry->num_pages -= 4;
        }
    }

    scratch_pd[511] = (unsigned long)&stack_pt | 0x3;

    // Setup current address space mapping -- this is just temporary, but needed
    // otherwise we'll get an immediate page-fault when %cr3 is modified.
    pml4e[0] = prev_pml4[0];
    return &pml4e;
}
