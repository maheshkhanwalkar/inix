ENTRY(_start)

SECTIONS
{
    . = 0x80000;
    .text ALIGN(4K):
    {
        KEEP(*(.text.boot))
        *(.text)
        *(.text.*)
    }

    .rodata ALIGN(4K):
    {
        *(.rodata)
        *(.rodata.*)
    }

    .data ALIGN(4K):
    {
        *(.data)
        *(.data.*)
    }

    .bss ALIGN(4K):
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
    }
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
}
