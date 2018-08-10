#include <arch/x86_64/include/mm/phys.h>
#include <arch/x86_64/include/lock/spinlock.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static struct multiboot_mmap_entry map[32];
static struct zone b_kernel, low_mem, high_mem;
static const uint64_t page_size = 0x1000;

static inline bool contains_obj(size_t index, uint64_t obj_start, uint64_t obj_end)
{
	struct multiboot_mmap_entry* entry = &map[index];
	return entry->addr <= obj_start && entry->addr + entry->len >= obj_end;
}

static inline uint64_t align_by(uint64_t addr, uint64_t align)
{
	uint64_t mask = ~(align - 1);
	uint64_t below = addr & mask;

	return below == addr ? addr : below + align;
}

bool phys_init(struct multiboot_info* info, uint64_t vma, uint64_t k_start, uint64_t k_end)
{
	// No memory map
	if((info->flags & 64) == 0)
		return false;

	// Memory map too large
	if(info->mmap_length > sizeof(map))
		return false;

	uint64_t ptr = info->mmap_addr + vma;
	size_t e_count = info->mmap_length / sizeof(struct multiboot_mmap_entry);

	for(size_t s = 0; s < e_count; s++)
		map[s] = ((struct multiboot_mmap_entry*)ptr)[s];

	bool first = false;
	uint64_t k_pos = 0, low_lim = 0;

	for(size_t s = 0; s < e_count; s++)
	{
		if(contains_obj(s, k_start, k_end))
		{
			uint64_t align = align_by(k_end, 0x100000);

			map[s].len -= align - map[s].addr;
			map[s].addr = align;

			k_pos = s;
		}
		else
		{
			uint64_t align = align_by(map[s].addr, page_size);

			map[s].len -= align - map[s].addr;
			map[s].addr = align;
		}

		if(map[s].addr >= 0x100000000 && !first)
		{
			low_lim = s;
			first = true;
		}
	}

	// Initialize zones
	b_kernel.map = map;
	b_kernel.start = 0;
	b_kernel.end = k_pos;

	low_mem.map = map;
	low_mem.start = k_pos;
	low_mem.end = low_lim;

	high_mem.map = map;
	high_mem.start = low_lim;
	high_mem.end = e_count;

	return true;
}

static inline struct zone* zone_select(enum phys_mem_zone zone)
{
	struct zone* zt;

	switch(zone)
	{
		case PHYS_BELOW_KERNEL:
			zt = &b_kernel;
			break;

		case PHYS_LOW_MEM:
			zt = &low_mem;
			break;

		case PHYS_HIGH_MEM:
			zt = &high_mem;
			break;

		default:
			zt = NULL;
			break;
	}

	return zt;
}

uint64_t phys_get_frame(enum phys_mem_zone zone)
{
	return phys_carve(zone, page_size);
}

uint64_t phys_carve(enum phys_mem_zone zone, uint64_t amt)
{
	struct zone* zt = zone_select(zone);

	uint64_t request = align_by(amt, page_size);
	return zone_get_chunk(zt, request);
}
