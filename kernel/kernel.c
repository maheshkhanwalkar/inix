#include <mm/alloc.h>
#include <inix/irq.h>

void kernel_main(void)
{
    vm_init();
    irq_arch_setup();
}
