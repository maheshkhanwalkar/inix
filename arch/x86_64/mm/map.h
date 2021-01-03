#pragma once

enum memory_map_flags {
    PHYS_RESERVED,
    PHYS_NORMAL_MEMORY,
    PHYS_FIRMWARE_RESERVED,
    PHYS_FIRMWARE_RECLAIMABLE,
    PHYS_ACPI_RESERVED,
    PHYS_ACPI_RECLAIMABLE,
    PHYS_MMIO_RESERVED,
    PHYS_UNUSABLE
};

typedef struct memory_map_entry {
    enum memory_map_flags flags;
    unsigned long phys_addr;
    unsigned long num_bytes;
} memory_map_entry_t;

typedef struct memory_map {
    memory_map_entry_t* entries;
    unsigned long count;
} memory_map_t;
