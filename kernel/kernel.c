#include <mm/alloc.h>

void kernel_main()
{
    vm_init();

    char* buffer = vm_alloc(256);
    buffer[0] = 'h';

    char* next = vm_alloc(256);
    next[0] = 'e';
}
