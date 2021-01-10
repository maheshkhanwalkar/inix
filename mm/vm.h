#pragma once
#include <inix/defs.h>

#define VM_PG_PRESENT 0x1
#define VM_PG_WRITE   0x2
#define VM_PG_EXECUTE 0x4
#define VM_PG_USER    0x8

ptr_t vm_carve(ptr_t pages);
void vm_map_page(ptr_t v_addr, ptr_t phys_addr, uint32_t flags);
void vm_invlpg(ptr_t v_addr);
