.code64

.section .text
.globl _entry64
_entry64:
    cli
    hlt


.section .boot_params, "aw"
boot_params:
    .skip 0x1000
