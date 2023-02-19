#include <inix/mm/alloc.h>
#include <inix/mm/phys.h>
#include <inix/mm/vm.h>
#include <inix/defs.h>
#include <inix/mm/paging.h>

#include <mm/alloc/page.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static const ptr_t VM_INITIAL_BUFFER = 0x8000;
static const ptr_t VM_CHECKSUM = 0xDEAD;

struct free_list_hdr;
struct free_list_ftr;

typedef struct free_list_hdr {
    // Doubly-linked
    struct free_list_hdr* next;
    struct free_list_hdr* prev;

    // Entry footer
    struct free_list_ftr* footer;

    // Metadata
    bool free;
    ptr_t size;
    ptr_t checksum;
} free_list_hdr_t;

typedef struct free_list_ftr {
    // Entry header and checksum
    struct free_list_hdr* header;
    ptr_t checksum;
} free_list_ftr_t;

typedef struct free_list {
    ptr_t size;
    struct free_list* next;
    ptr_t checksum;
} free_list_t;

static free_list_t free_head;

static void* allocate(ptr_t pages)
{
    ptr_t virt = vm_carve(pages);

    if(!virt) {
        panic("exhausted virtual address space");
    }

    for(ptr_t i = 0; i < pages; i++) {
        uint32_t flags = VM_PG_PRESENT | VM_PG_WRITE;

        ptr_t phys = phys_allocate_frame();

        if(phys == 0) {
            panic("out of memory");
        }

        vm_map_page(virt + i * VM_PAGE_SIZE, phys, flags);
        vm_invlpg(virt + i * VM_PAGE_SIZE);
    }

    return (void*)virt;
}

static void* create_entry(ptr_t actual, free_list_t* after)
{
    ptr_t pages = DIV_UP(actual, VM_PAGE_SIZE);
    free_list_t* next = allocate(pages);

    // Can split up and add an entry to the freelist
    if(pages * VM_PAGE_SIZE >= actual + sizeof(free_list_t)) {
        next->size = actual;
        next->next = NULL;
        next->checksum = VM_CHECKSUM;

        free_list_t* free = (free_list_t*)((uint8_t*)next + actual);
        free->size = pages * VM_PAGE_SIZE - actual;
        free->next = after;
        free->checksum = VM_CHECKSUM;

        free_head.next = free;
        return next + 1;

    } else {
        next->size = pages * VM_PAGE_SIZE;
        next->next = NULL;
        next->checksum = VM_CHECKSUM;

        return next + 1;
    }
}

static ptr_t compute_pad(ptr_t amt)
{
    ptr_t ptr_size = sizeof(ptr_size);
    return ptr_size - (amt & (ptr_size - 1));
}

void vm_init(void)
{
    // Create initial pool of memory
    ptr_t pages = DIV_UP(VM_INITIAL_BUFFER, VM_PAGE_SIZE);
    free_list_t* next = allocate(pages);

    next->size = pages * VM_PAGE_SIZE;
    next->next = 0;
    next->checksum = VM_CHECKSUM;

    free_head.size = 0;
    free_head.next = next;

    // Initialise paging subcomponent
    vm_page_init();
}

void* vm_alloc(ptr_t amt)
{
    ptr_t actual = amt + sizeof(free_list_t) + compute_pad(amt);

    if(!free_head.next) {
        return create_entry(actual, NULL);
    }

    free_list_t* curr = free_head.next;
    free_list_t* prev = &free_head;

    while(curr) {
        if(curr->size < actual) {
            curr = curr->next;
            prev = curr;
            continue;
        }

        if(curr->size >= actual + sizeof(free_list_t)) {
            curr->size -= actual;

            free_list_t* split = (free_list_t*)((uint8_t*)curr + curr->size);

            split->size = actual;
            split->next = NULL;
            split->checksum = VM_CHECKSUM;

            return split + 1;
        }

        prev->next = curr->next;
        return curr + 1;
    }

    // Couldn't find anything -- need to allocate
    return create_entry(actual, free_head.next);
}

void vm_free(void* ptr)
{
    if(!ptr)
        return;

    // TODO - this current implementation will lead to fragmentation, which
    //  is not ideal -- coalescing support is needed!

    free_list_t* meta = (free_list_t*)ptr - 1;

    // Check for potential corruption
    if(meta->checksum != VM_CHECKSUM) {
        panic("heap corruption");
    }

    meta->next = free_head.next;
    free_head.next = meta;
}

void vm_donate(void* ptr, ptr_t amt)
{
    // Either not valid or sadly not enough space -- in either case, just simply
    // ignore the donation request because it is of no use...
    if(!ptr || amt <= sizeof(free_list_t))
        return;

    free_list_t* meta = (free_list_t*)ptr;

    meta->size = amt;
    meta->checksum = VM_CHECKSUM;
    meta->next = free_head.next;
    free_head.next = meta;
}
