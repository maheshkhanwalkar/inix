#include <stdint.h>
#include <inix/elf.h>
#include <arch/x86_64/efi/memory.h>
#include <arch/x86_64/mm/pml4.h>
#include <arch/x86_64/mm/paging.h>
#include <arch/x86_64/mm/map.h>

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
    memory_map_t map;
    unsigned long kernel_start, kernel_end;
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

static unsigned long find_free_pages(mem_map_t* mem_map, unsigned long pages)
{
    for(unsigned long i = 0; i < mem_map->count; i++) {
        efi_memory_entry_t* entry = &mem_map->map[i];

        if(entry->type != EFI_CONVENTIONAL_MEMORY || entry->num_pages <= pages)
            continue;

        // Don't select low memory for these pages -- we want to keep them for other
        // purposes (if needed)
        if(entry->phys_start < 0x100000)
            continue;

        unsigned long start = entry->phys_start;

        entry->phys_start += pages * PAGE_SIZE;
        entry->num_pages -= pages;

        return start;
    }

    return 0;
}

static void setup_kernel_tables(unsigned long kern_base, kern_img_t* kern_img)
{
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
            unsigned long nx = (hdr->p_flags & PF_X) ? 0 : PG_NO_EXECUTE;
            unsigned long wr = (hdr->p_flags & PF_W) && nx ? PG_WRITE : 0;

            // Setup entry: address and flags
            pt[pt_pos][page_pos] = address | wr | nx | PG_GLOBAL | PG_PRESENT;
            page_pos++;
        }
    }

    boot_params.kernel_start = 0xFFFFFFFF80000000;
    boot_params.kernel_end = boot_params.kernel_start + (pt_pos * PT_NUM_ENTRIES + page_pos) * PAGE_SIZE;

    // Setup higher-level paging structures
    // Assumption: kernel vma is hard-coded at 0xFFFFFFFF80000000 -- FIXME!
    for(unsigned long i = 0; i < PT_NUM_ENTRIES; i++) {
        pd[i] = (unsigned long)&pt[i] | PG_WRITE | PG_PRESENT;
    }

    pdpt[PT_NUM_ENTRIES - 2] = (unsigned long)&pd | PG_WRITE | PG_PRESENT;
    pml4e[PT_NUM_ENTRIES - 1] = (unsigned long)&pdpt | PG_WRITE | PG_PRESENT;
}

static void setup_memory_map(mem_map_t* mem_map)
{
    unsigned long pages = (mem_map->count * sizeof(memory_map_entry_t) + PAGE_SIZE - 1) / PAGE_SIZE;
    unsigned long start = find_free_pages(mem_map, pages);

    uint64_t pt_index = PT_INDEX(boot_params.kernel_end);
    uint64_t pd_index = PD_INDEX(boot_params.kernel_end);

    for(unsigned long p = 0; p < pages; p++) {
        if(pt_index == PT_NUM_ENTRIES) {
            pt_index = 0;
            pd_index++;
        }

        unsigned long flags = PG_PRESENT | PG_GLOBAL | PG_WRITE | PG_NO_EXECUTE;
        pt[pd_index][pt_index] = (start + p * PAGE_SIZE) | flags;
    }

    boot_params.map.count = mem_map->count;
    boot_params.map.entries = (memory_map_entry_t*)boot_params.kernel_end;
    boot_params.kernel_end += pages * PAGE_SIZE;
}

static enum memory_map_flags convert_flags(unsigned long type)
{
    switch ((enum efi_memory_type)type) {
        case EFI_RESERVED:
        case EFI_PAL_CODE:
            return PHYS_RESERVED;

        case EFI_CONVENTIONAL_MEMORY:
            return PHYS_NORMAL_MEMORY;

        case EFI_BOOT_SERVICES_CODE:
        case EFI_BOOT_SERVICES_DATA:
        case EFI_LOADER_CODE:
        case EFI_LOADER_DATA:
            return PHYS_FIRMWARE_RECLAIMABLE;

        case EFI_RUNTIME_SERVICES_CODE:
        case EFI_RUNTIME_SERVICES_DATA:
            return PHYS_FIRMWARE_RESERVED;

        case EFI_UNUSABLE_MEMORY:
            return PHYS_UNUSABLE;

        case EFI_ACPI_RECLAIMABLE:
            return PHYS_ACPI_RECLAIMABLE;
        case EFI_ACPI_NVS:
            return PHYS_ACPI_RESERVED;

        case EFI_MMIO:
        case EFI_MMIO_PORT_SPACE:
            return PHYS_MMIO_RESERVED;
    }

    if(type == EFI_CONVENTIONAL_MEMORY)
        return PHYS_NORMAL_MEMORY;

    return PHYS_RESERVED;
}

static void setup_stack(mem_map_t* mem_map)
{
    // Setup scratch space tables
    scratch_pd[0] = (unsigned long)&scratch_pt | PG_WRITE | PG_PRESENT;
    pdpt[PT_NUM_ENTRIES - 1] = (unsigned long)&scratch_pd | PG_WRITE | PG_PRESENT;

    // Setup stack page table (16k stack)
    unsigned long stack_bottom = find_free_pages(mem_map, STACK_NUM_PAGES);

    for(int j = 0; j < STACK_NUM_PAGES; j++) {
        unsigned long address = stack_bottom + PAGE_SIZE * j;
        unsigned long flags = PG_NO_EXECUTE | PG_WRITE | PG_GLOBAL | PG_PRESENT;
        stack_pt[(PT_NUM_ENTRIES - STACK_NUM_PAGES) + j] = address | flags;
    }

    scratch_pd[PT_NUM_ENTRIES - 1] = (unsigned long)&stack_pt | PG_WRITE | PG_PRESENT;
}

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
    setup_kernel_tables(kern_base, kern_img);
    setup_memory_map(mem_map);
    setup_stack(mem_map);

    // Setup current address space mapping -- this is just temporary, but needed
    // otherwise we'll get an immediate page-fault when %cr3 is modified.
    pml4e[0] = prev_pml4[0];
    return &pml4e;
}

/**
 * Transform and copy the EFI memory map into the boot-independent format, which was
 * allocated and mapped in by setup_memory_map()
 *
 * The address stored in boot_params.map.entries is in the higher-half, while the
 * mem_map->map address is in the lower half. This function is called after the new page
 * tables have come into effect, but before the lower half is unmapped.
 *
 * See arch/x86_64/boot/boot.S for additional context
 */
void copy_memory_map(mem_map_t* mem_map)
{
    for(unsigned long i = 0; i < boot_params.map.count; i++) {
        boot_params.map.entries[i].phys_addr = mem_map->map[i].phys_start;
        boot_params.map.entries[i].num_bytes = mem_map->map[i].num_pages * PAGE_SIZE;
        boot_params.map.entries[i].flags = convert_flags(mem_map->map[i].type);
    }
}
