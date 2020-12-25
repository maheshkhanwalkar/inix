OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_entry64)

SECTIONS
{
    . = 1M;

    kernel_vma = 0xFFFFFFFF80000000;
    . += kernel_vma;
}
