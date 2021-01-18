#include <inix/mm/alloc.h>
#include <inix/irq.h>

void kernel_main(void)
{
    vm_init();
    arch_irq_setup();
}
