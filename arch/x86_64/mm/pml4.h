#pragma once
#define PT_NUM_ENTRIES 512
#define PAGE_SIZE 0x1000

#define PAGE_ALIGN(addr) ((addr) & ~(PAGE_SIZE - 1UL))

#define PT_INDEX(addr) (((addr) / PAGE_SIZE) % PT_NUM_ENTRIES)
#define PD_INDEX(addr) (((addr) / PAGE_SIZE / PT_NUM_ENTRIES) % PT_NUM_ENTRIES)
#define PDPT_INDEX(addr) (((addr) / PAGE_SIZE / PT_NUM_ENTRIES / PT_NUM_ENTRIES) % PT_NUM_ENTRIES)
#define PML4_INDEX(addr) ((addr) / PAGE_SIZE / PT_NUM_ENTRIES / PT_NUM_ENTRIES / PT_NUM_ENTRIES)
