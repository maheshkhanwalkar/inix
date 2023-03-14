#pragma once

enum efi_memory_type {
    EFI_RESERVED,
    EFI_LOADER_CODE,
    EFI_LOADER_DATA,
    EFI_BOOT_SERVICES_CODE,
    EFI_BOOT_SERVICES_DATA,
    EFI_RUNTIME_SERVICES_CODE,
    EFI_RUNTIME_SERVICES_DATA,
    EFI_CONVENTIONAL_MEMORY,
    EFI_UNUSABLE_MEMORY,
    EFI_ACPI_RECLAIMABLE,
    EFI_ACPI_NVS,
    EFI_MMIO,
    EFI_MMIO_PORT_SPACE,
    EFI_PAL_CODE
};

#define EFI_PAGE_SIZE 0x1000

typedef struct efi_memory_entry {
    unsigned int type;
    unsigned long phys_start;
    unsigned long virt_start;
    unsigned long num_pages;
    unsigned long attribute;
    unsigned long padding;
} efi_memory_entry_t;
