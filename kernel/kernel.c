#include <mm/alloc.h>
#include <inix/irq.h>
#include <inix/mm/phys.h>

void kernel_main()
{
    ptr_t frame = phys_allocate_frame();
    phys_free_frame(frame);

    //vm_init();
    //irq_arch_setup();
}
