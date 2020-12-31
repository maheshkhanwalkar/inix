#pragma once

#define EFI_CONVENTIONAL_MEMORY 0x7
#define EFI_PAGE_SIZE 0x1000

typedef struct efi_memory_entry {
    unsigned int type;
    unsigned long phys_start;
    unsigned long virt_start;
    unsigned long num_pages;
    unsigned long attribute;
    unsigned long padding;
} efi_memory_entry_t;
