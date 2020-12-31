#include <stdint.h>
#include <arch/x86_64/mm/pml4.h>

// Base page tables
uint64_t pml4e[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t pdpt[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t pd[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t pt[PT_NUM_ENTRIES][PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));

// Scratch space tables
uint64_t scratch_pd[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
uint64_t scratch_pt[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));

// Stack page table
uint64_t stack_pt[PT_NUM_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
