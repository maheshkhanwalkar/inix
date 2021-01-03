#pragma once

typedef enum phys_zone {
    /* All other memory */
    PHYS_ZONE_NORMAL,
    /* Memory < 1 MiB */
    PHYS_ZONE_LOW_MEMORY
} phys_zone_t;
