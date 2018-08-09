#pragma once

#include <arch/x86_64/include/boot/multiboot.h>

#include <stdint.h>
#include <stdbool.h>

#define PHYS_BAD_ALLOC 0xFFFFFFFFFFFFFFFF

enum phys_mem_zone
{
	/* Memory below the kernel */
	PHYS_BELOW_KERNEL,
	/* Memory above kernel, but below 4 GiB */
	PHYS_LOW_MEM,
	/* Memory above 4 GiB */
	PHYS_HIGH_MEM,
	/* Memory above the kernel */
	PHYS_NORMAL,
};

/* Initialize the physical memory allocator */
bool phys_init(struct multiboot_info* info, uint64_t vma,
		uint64_t k_start, uint64_t k_end);

/* Retrieve a page frame from the indicated 'zone' */
uint64_t phys_get_frame(enum phys_mem_zone zone);

/*
 * Carve out a (potentially multi-page) region from the
 * indicated 'zone' that is large enough to hold 'amt' bytes
 */
uint64_t phys_carve(enum phys_mem_zone zone, uint64_t amt);
