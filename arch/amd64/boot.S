.code32

# Multiboot header constants
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Embedded Multiboot Header
.section .multiboot,"aw",@progbits
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bstrapL,"ax",@progbits
.globl _low_init

_low_init:
    xchgw %bx, %bx

    # Setup bootstrap stack
    movl $bstack_top, %esp
    pushl %ebx

    # Setup 4-table paging
    call _setup_paging  # paging.S

    # Enable paging
    call _enable_paging # paging.S

    cli
    hlt

.section .bstack,"aw",@progbits
bstack_bottom:
.skip 128
bstack_top:

.code64

.section .bstrapH,"ax",@progbits
.globl _high_init

_high_init:
    cli
    hlt

