#include <arch/x86_64/boot/params.h>

/**
 * Boot parameters
 */
static boot_params_t boot_params;

boot_params_t* get_boot_params(void)
{
    return &boot_params;
}
