#include <stddef.h>
#include <stdbool.h>

#include <arch/amd64/include/multiboot.h>
#include <include/boot/entry.h>

// Kernel memory information
struct mem_info
{
    struct multiboot_info* info;
    uint64_t kernel_start, kernel_end;
    uint64_t kernel_vma;
};

// Storage
struct mem_entry entries[16];
struct initrd_entry initrd;
struct boot_info b_info;

static inline bool contains_kernel(struct mem_info* m_info,
        struct multiboot_mmap_entry* curr)
{
    return curr->addr <= m_info->kernel_start &&
        curr->addr + curr->len > m_info->kernel_start;
}

static inline void* fix_addr(void* addr, uint64_t kernel_vma)
{
    return (char*)addr + kernel_vma;
}

struct boot_info* parse_multiboot(struct mem_info* m_info)
{
    struct multiboot_info* info = m_info->info;

    // No memory map
    if((info->flags & 0x40) == 0)
        return NULL;

    // No initramfs
    if((info->flags & 0x8) == 0)
        return NULL;

    // Extract memory map
    struct multiboot_mmap_entry* map = fix_addr((void*)info->mmap_addr, m_info->kernel_vma);
    size_t map_len = info->mmap_length / sizeof(struct multiboot_mmap_entry);

    if(map_len > 15)
        return NULL;

    bool extra = false;

    for(size_t s = 0, pos = 0; s < map_len; s++, pos++)
    {
        if(contains_kernel(m_info, &map[s]))
        {
            entries[pos].start_addr = (void*)map[s].addr;
            entries[pos].length = map[s].len;
            entries[pos].native_type = MULTIBOOT_MEMORY_AVAILABLE;
            entries[pos].type = MEM_USED;

            bool start = false;

            if(map[s].addr < m_info->kernel_start)
            {
                entries[pos].length = m_info->kernel_start - map[s].addr;
                entries[pos].type = MEM_FREE;

                start = true;
            }

            if(map[s].addr + map[s].len > m_info->kernel_end)
            {
                if(start)
                {
                    pos++;
                    extra = true;
                }

                entries[pos].start_addr = (void*)m_info->kernel_end;
                entries[pos].length = map[s].addr + map[s].len - m_info->kernel_end;
                entries[pos].native_type = MULTIBOOT_MEMORY_AVAILABLE;
                entries[pos].type = MEM_FREE;
            }
        }
        else
        {
            entries[pos].start_addr = (void*)map[s].addr;
            entries[pos].length = map[s].len;
            entries[pos].native_type = map[s].type;

            switch(map[s].type)
            {
                case MULTIBOOT_MEMORY_AVAILABLE:
                    entries[pos].type = MEM_FREE;
                    break;
                case MULTIBOOT_MEMORY_BADRAM:
                    entries[pos].type = MEM_BAD;
                    break;
                case MULTIBOOT_MEMORY_RESERVED:
                    entries[pos].type = MEM_USED;
                    break;
                default:
                    entries[pos].type = MEM_SPECIAL;
                    break;
            }
        }
    }

    // Extract initrd information
    struct multiboot_mod_list* m_list = fix_addr((void*)info->mods_addr, m_info->kernel_vma);

    initrd.start_addr = (char*)m_list->mod_start + m_info->kernel_vma;
    initrd.length = m_list->mod_end - m_list->mod_start;

    size_t e_count = extra ? map_len + 1 : map_len;
    char* k_args = fix_addr((void*)info->cmdline, m_info->kernel_vma);

    b_info.entries = entries;
    b_info.e_count = e_count;
    b_info.initrd = &initrd;
    b_info.k_args = k_args;

    return &b_info;
}
