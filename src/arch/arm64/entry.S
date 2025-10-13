.section ".text.boot"
.globl _start

.org 0x80000
_start:
    // setup stack
    ldr x4, =_start
    mov sp, x4

    // clear BSS
    ldr x4, =__bss_start
    ldr w5, =__bss_size
clear_bss:
    cbz w5, ready
    str xzr, [x4], #8   // also handles x4 increment
    sub w5, w5, #1
    cbnz w5, clear_bss

ready:
    bl kernel_main

halt:
    wfe
    b halt
