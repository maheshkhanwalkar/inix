#include <mm/alloc.h>
#include <inix/irq.h>

void kernel_main()
{
    vm_init();
    irq_arch_setup();
}
