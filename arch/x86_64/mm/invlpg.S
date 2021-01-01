.section .text
.globl _invlpg

_invlpg:
    invlpg (%rdi)
    retq
