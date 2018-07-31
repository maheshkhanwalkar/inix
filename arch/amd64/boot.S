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
    # Setup bootstrap stack
    movl $bstack_top, %esp
    pushl %ebx

    # Setup 4-table paging
    call _setup_paging  # paging.S

    # Enable paging
    call _enable_paging # paging.S

    # Load 64-bit GDT
    lgdt gdtr           # gdt.S

    # Far jump to set CS
    ljmp $0x08,$_jmp_target

.section .bstack,"aw",@progbits
bstack_bottom:
.skip 128
bstack_top:

.code64

.section .bstrapL,"ax",@progbits
.globl _jmp_target

_jmp_target:
    # TODO setup DS, SS, ES, GS, and FS registers
    # TODO setup a new higher-half stack
    jmp _high_init

.section .bstrapH,"ax",@progbits
.globl _high_init

_high_init:
    xchgw %bx, %bx

    cli
    hlt

