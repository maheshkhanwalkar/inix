#include <inix/defs.h>

static const ptr_t VM_CHECKSUM = 0xDEAD;

typedef struct page_list {
    struct page_list* next;
    ptr_t checksum;
} page_list_t;

static page_list_t page_head;
