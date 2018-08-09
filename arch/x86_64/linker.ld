/* Linker Script */

ENTRY(_low_init)

SECTIONS
{
    . = 1M;

    kernel_start = .;
    kernel_vma = 0xFFFFFFFF80000000;

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

    . += kernel_vma;

    .bstrapH ALIGN(4K): AT(ADDR(.bstrapH) - kernel_vma)
    {
        *(.bstrapH)
    }

    .text ALIGN(4K): AT(ADDR(.text) - kernel_vma)
    {
        *(.text)
    }

    .rodata ALIGN(4K): AT(ADDR(.rodata) - kernel_vma)
    {
        *(.rodata)
    }

    .data ALIGN(4K): AT(ADDR(.data) - kernel_vma)
    {
        *(.data)
    }

    .bss ALIGN(4K): AT(ADDR(.bss) - kernel_vma)
    {
        *(COMMON)
        *(.bss)
    }

    .hstack ALIGN(16): AT(ADDR(.hstack) - kernel_vma)
    {
        *(.hstack)
    }

    /DISCARD/ :
    {
        *(.comment)
    }

    kernel_end = . - kernel_vma;
}
