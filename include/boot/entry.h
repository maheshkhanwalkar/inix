#include <stddef.h>
#include <stdint.h>

/* Generic memory types */
enum mem_entry_type
{
    MEM_FREE,
    MEM_USED,
    MEM_BAD,
    MEM_SPECIAL
};

struct mem_entry
{
    void* start_addr;
    uint64_t length;

    enum mem_entry_type type;
    uint32_t native_type;

};

struct initrd_entry
{
    void* start_addr;
    uint64_t length;
};

struct boot_info
{
    struct mem_entry* entries;
    size_t e_count;

    struct initrd_entry* initrd;
    char* k_args;
};
