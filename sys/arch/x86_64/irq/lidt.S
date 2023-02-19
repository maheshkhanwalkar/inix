.section .text
.globl _lidt
_lidt:
    lidt (%rdi)
    retq
