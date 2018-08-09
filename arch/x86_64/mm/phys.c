#include <arch/x86_64/include/mm/phys.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct zone_data
{
	uint64_t k_zone;
	uint64_t low_limit;
};

static struct multiboot_mmap_entry map[32];
static struct zone_data z_data;

static size_t e_count;
static uint64_t page_size = 0x1000;

static inline bool contains_obj(size_t index, uint64_t obj_start, uint64_t obj_end)
{
	if(index >= e_count)
		return false;

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
	e_count = info->mmap_length / sizeof(struct multiboot_mmap_entry);

	for(size_t s = 0; s < e_count; s++)
		map[s] = ((struct multiboot_mmap_entry*)ptr)[s];

	bool first = false;

	for(size_t s = 0; s < e_count; s++)
	{
		if(contains_obj(s, k_start, k_end))
		{
			uint64_t align = align_by(k_end, 0x100000);

			map[s].len -= align - map[s].addr;
			map[s].addr = align;

			z_data.k_zone = s;
		}
		else
		{
			uint64_t align = align_by(map[s].addr, page_size);

			map[s].len -= align - map[s].addr;
			map[s].addr = align;
		}

		if(map[s].addr >= 0x100000000 && !first)
		{
			z_data.low_limit = s;
			first = true;
		}
	}

	return true;
}

static inline void zone_select(enum phys_mem_zone zone, size_t* const start,
		size_t* const end)
{
	switch(zone)
	{
		case PHYS_BELOW_KERNEL:
			*start = 0;
			*end = z_data.k_zone;
			break;

		case PHYS_LOW_MEM:
			*start = z_data.k_zone;
			*end = z_data.low_limit;
			break;

		case PHYS_HIGH_MEM:
			*start = z_data.low_limit;
			*end = e_count;
			break;

		case PHYS_NORMAL:
			*start = z_data.k_zone;
			*end = e_count;
			break;

		/* Invalid */
		default:
			*start = 0;
			*end = 0;
			break;
	}
}

uint64_t phys_get_frame(enum phys_mem_zone zone)
{
	uint64_t start, end;
	zone_select(zone, &start, &end);

	for(size_t s = start; s < end; s++)
	{
		struct multiboot_mmap_entry* curr = &map[s];

		if(curr->type != MULTIBOOT_MEMORY_AVAILABLE)
			continue;

		if(curr->len < page_size)
			continue;

		uint64_t frame = curr->addr;

		curr->addr += page_size;
		curr->len -= page_size;

		return frame;
	}

	return PHYS_BAD_ALLOC;
}
