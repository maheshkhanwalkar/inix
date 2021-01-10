#include <mm/alloc.h>
#include <inix/mm/phys.h>
#include <mm/vm.h>
#include <inix/defs.h>

#include <stdint.h>
#include <stddef.h>

#define VM_INITIAL_BUFFER 0x8000
#define VM_CHECKSUM 0xDEAD

typedef struct free_list {
    uintptr_t size;
    struct free_list* next;
    uintptr_t checksum;
} free_list_t;

free_list_t free_head;

static void* allocate(uintptr_t pages)
{
    uintptr_t phys = 0;//phys_carve(pages, 0);
    uintptr_t virt = vm_carve(pages);

    if(!phys || !virt) {
        panic();
    }

    for(uintptr_t i = 0; i < pages; i++) {
        uint32_t flags = VM_PG_PRESENT | VM_PG_WRITE;

        vm_map_page(virt + i * vm_page_size(), phys + i * vm_page_size(), flags);
        vm_invlpg(virt + i * vm_page_size());
    }

    return (void*)virt;
}

static void* create_entry(uintptr_t actual, free_list_t* after)
{
    uintptr_t pages = DIV_UP(actual, vm_page_size());
    free_list_t* next = allocate(pages);

    // Can split up and add an entry to the freelist
    if(pages * vm_page_size() >= actual + sizeof(free_list_t)) {
        next->size = actual;
        next->next = NULL;
        next->checksum = VM_CHECKSUM;

        free_list_t* free = (free_list_t*)((uint8_t*)next + actual);
        free->size = pages * vm_page_size() - actual;
        free->next = after;
        free->checksum = VM_CHECKSUM;

        free_head.next = free;
        return next + 1;

    } else {
        next->size = pages * vm_page_size();
        next->next = NULL;
        next->checksum = VM_CHECKSUM;

        return next + 1;
    }
}

static uintptr_t compute_pad(uintptr_t amt)
{
    uintptr_t ptr_size = sizeof(uintptr_t);
    return ptr_size - (amt & (ptr_size - 1));
}

void vm_init()
{
    uintptr_t pages = DIV_UP(VM_INITIAL_BUFFER, vm_page_size());
    free_list_t* next = allocate(pages);

    next->size = pages * vm_page_size();
    next->next = 0;
    next->checksum = VM_CHECKSUM;

    free_head.size = 0;
    free_head.next = next;
}

void* vm_alloc(uintptr_t amt)
{
    uintptr_t actual = amt + sizeof(free_list_t) + compute_pad(amt);

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
        panic();
    }

    meta->next = free_head.next;
    free_head.next = meta;
}
