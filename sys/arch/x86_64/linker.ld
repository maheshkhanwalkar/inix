OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_entry64)

SECTIONS
{
    kernel_vma = 0xFFFFFFFF80000000;
    . = kernel_vma;

    .text ALIGN(4K):
    {
        *(.text)
    }

    .data: ALIGN(4K):
    {
        *(.data)
    }

    .rodata: ALIGN(4K):
    {
       *(.rodata)
    }

    .bss: ALIGN(4K):
    {
        *(COMMON)
        *(.bss)
    }
}
