/**
 * Physical memory management
 *
 * In the hosted environment, there is no real concept of physical memory, so the
 * allocation/freeing does not do anything.
 *
 * Instead, the actual allocation work is done within the virtual memory subsystem,
 * which makes calls to the standard library to get memory to work with.
 */

#include <inix/defs.h>

ptr_t arch_phys_allocate_frame(void) { return 0; }
void arch_phys_free_frame(ptr_t frame __unused) { }
