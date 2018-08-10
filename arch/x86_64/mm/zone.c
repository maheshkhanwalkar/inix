#include <arch/x86_64/include/mm/zone.h>

#include <stddef.h>
#include <stdint.h>

uint64_t zone_get_chunk(struct zone* zt, uint64_t amt)
{
	if(zt == NULL)
		return ZONE_BAD_ALLOC;

	uint64_t frame = ZONE_BAD_ALLOC;
	spinlock_acquire(zt->lock);

	for(size_t s = zt->start; s < zt->end; s++)
	{
		struct multiboot_mmap_entry* curr = &zt->map[s];

		if(curr->type != MULTIBOOT_MEMORY_AVAILABLE)
			continue;

		if(curr->len < amt)
			continue;

		frame = curr->addr;

		curr->addr += amt;
		curr->len -= amt;

		break;
	}

	spinlock_release(zt->lock);
	return frame;
}
