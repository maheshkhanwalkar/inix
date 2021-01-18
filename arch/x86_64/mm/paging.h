#pragma once

#define PG_PRESENT    0x1
#define PG_WRITE      0x2
#define PG_GLOBAL     0x100
#define PG_NO_EXECUTE (1UL << 63)
#define PG_USER       0x4

#define PAGE_SIZE 0x1000UL
