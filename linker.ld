/* Linker Script */

ENTRY(_low_init)

SECTIONS
{
    . = 1M;

    _kernel_start = .;
    _kernel_vma = 0xFFFFFFFF80000000;

    .multiboot ALIGN(4K):
    {
        *(.multiboot)
    }

    .bstrapL ALIGN(4K):
    {
        *(.bstrapL)
    }

    .paging ALIGN(4K):
    {
        *(.paging)
    }

    .gdt ALIGN(8):
    {
        *(.gdt)
    }

    .bstack ALIGN(16):
    {
        *(.bstack)
    }

    . += _kernel_vma;

    .bstrapH ALIGN(4K): AT(ADDR(.bstrapH) - _kernel_vma)
    {
        *(.bstrapH)
    }

    .text ALIGN(4K): AT(ADDR(.text) - _kernel_vma)
    {
        *(.text)
    }

    .rodata ALIGN(4K): AT(ADDR(.rodata) - _kernel_vma)
    {
        *(.rodata)
    }

    .data ALIGN(4K): AT(ADDR(.data) - _kernel_vma)
    {
        *(.data)
    }

    .bss ALIGN(4K): AT(ADDR(.bss) - _kernel_vma)
    {
        *(COMMON)
        *(.bss)
    }

    .hstack ALIGN(16): AT(ADDR(.hstack) - _kernel_vma)
    {
        *(.hstack)
    }

    /DISCARD/ :
    {
        *(.comment)
    }

    _kernel_end = . - _kernel_vma;
}
