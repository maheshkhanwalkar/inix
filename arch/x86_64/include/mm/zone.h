#pragma once

#include <stdint.h>

#include <arch/x86_64/include/lock/spinlock.h>
#include <arch/x86_64/include/boot/multiboot.h>

#define ZONE_BAD_ALLOC 0xFFFFFFFFFFFFFFFF

struct zone
{
	struct multiboot_mmap_entry* map;
	uint64_t start, end;
	spinlock_t lock;
};

/*
 * Retrieve a chunk of size 'amt' from the zone
 *
 * WARNING: 'amt' should be rounded up to the nearest page
 * size, otherwise, other calls to zone_get_chunk() will
 * return an unaligned address.
 */
uint64_t zone_get_chunk(struct zone* zt, uint64_t amt);
