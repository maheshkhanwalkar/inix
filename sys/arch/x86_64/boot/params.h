#pragma once
#include <arch/x86_64/mm/map.h>

typedef struct boot_params {
    memory_map_t map;
    unsigned long kernel_start, kernel_end;
    unsigned long stack_top, stack_bottom;
} boot_params_t;

/**
 * Get a pointer to the boot parameters section
 */
boot_params_t* get_boot_params(void);
