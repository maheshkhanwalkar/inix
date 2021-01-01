#pragma once

typedef enum phys_zone {
    /* Memory < 1 MiB */
    PHYS_ZONE_LOW_MEMORY,
    /* All other memory */
    PHYS_ZONE_NORMAL
} phys_zone_t;
