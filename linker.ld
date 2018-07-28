/* Linker Script */

ENTRY(_low_init)

SECTIONS
{
    . = 1M;
    KERNEL_VMA = 0xFFFFFFFF80000000;

    .multiboot ALIGN(4K):
    {
        *(.multiboot)
    }

    .bstrapL ALIGN(4K):
    {
        *(.bstrapL)
    }

    . += KERNEL_VMA;

    .bstrapH ALIGN(4K): AT(ADDR(.bstrapH) - KERNEL_VMA)
    {
        *(.bstrapH)
    }

    .text ALIGN(4K): AT(ADDR(.text) - KERNEL_VMA)
    {
        *(.text)
    }

    .rodata ALIGN(4K): AT(ADDR(.rodata) - KERNEL_VMA)
    {
        *(.rodata)
    }

    .data ALIGN(4K): AT(ADDR(.data) - KERNEL_VMA)
    {
        *(.data)
    }

    .bss ALIGN(4K): AT(ADDR(.bss) - KERNEL_VMA)
    {
        *(COMMON)
        *(.bss)
    }
}
