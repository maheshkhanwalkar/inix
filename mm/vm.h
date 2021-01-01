#pragma once
#include <stdint.h>

#define VM_PG_PRESENT 0x1
#define VM_PG_WRITE   0x2
#define VM_PG_EXECUTE 0x4
#define VM_PG_USER    0x8

uintptr_t vm_carve(uintptr_t pages);
void vm_map_page(uintptr_t v_addr, uintptr_t phys_addr, uint32_t flags);
void vm_invlpg(uintptr_t v_addr);
uint32_t vm_page_size();
