.section ".text.boot"
.globl _start

.org 0x80000
_start:
    // setup stack
    ldr x0, =_start
    mov sp, x0

    // clear BSS
    ldr x0, =__bss_start
    ldr w1, =__bss_size
clear_bss:
    cbz w1, ready
    str xzr, [x0], #8   // also handles x0 increment
    sub w1, w1, #1
    cbnz w1, clear_bss

ready:
    bl kernel_main

halt:
    wfe
    b halt
