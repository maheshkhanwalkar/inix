#include <inix/defs.h>
#include <inix/mm/paging.h>
#include <inix/mm/phys.h>
#include <inix/mm/vm.h>

static const ptr_t VM_INITIAL_BUFFER = 0x8000;

typedef struct page_list {
    struct page_list* next;
} page_list_t;

static page_list_t page_head;

static void* allocate(void)
{
    ptr_t phys = phys_allocate_frame();
    ptr_t virt = vm_carve(1);

    vm_map_page(virt, phys, VM_PG_PRESENT | VM_PG_WRITE);
    vm_invlpg(virt);

    return (void*)virt;
}

void vm_page_init(void)
{
    ptr_t pages = VM_INITIAL_BUFFER / VM_PAGE_SIZE;

    // Create an initial buffer of pages
    for(ptr_t i = 0; i < pages; i++) {
        page_list_t* next = (page_list_t*)allocate();

        next->next = page_head.next;
        page_head.next = next;
    }
}

void* vm_page_alloc(void)
{
    if(!page_head.next) {
        return allocate();
    }

    page_list_t* next = page_head.next;
    page_head.next = next->next;
    return next;
}

void vm_page_free(void* ptr)
{
    page_list_t* next = (page_list_t*)ptr;

    next->next = page_head.next;
    page_head.next = next;
}
