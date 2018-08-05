.code32

.section .gdt,"aw",@progbits
.globl gdt64
.globl gdtr

gdt64:
    # Null Segment
    .quad 0

    # Code Segment
    .short 0xFFFF
    .short 0
    .byte 0
    .byte 0b10011000 # P + DPL0 + Exec
    .byte 0b10111111 # G + L + AVL + Limit
    .byte 0

    # Data Segment
    .short 0xFFFF
    .short 0
    .byte 0
    .byte 0b10010010 # P + DPL0 + R/W
    .byte 0b10111111 # G + L + AVL + Limit
    .byte 0

gdtr:
    .short . - gdt64 - 1
    .quad gdt64

