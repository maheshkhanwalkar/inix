.code64

.section .text
.globl _entry64
_entry64:
    # Check for EFI magic
    cmpq $0xEF1B001, %rcx
    je _prep_efi_args

    # Loaded by something else -- not supported
    cli
    hlt

_prep_efi_args:
    # Disable interrupts, cleanup IDT
    cli
    lidtq 0

    # MS x64 ABI --> System V x86_64 ABI
    movq %rdx, %rdi
    movq %r8, %rsi
    movq %r9, %rdx
    movq %cr3, %rcx

    # Save kernel base and memory map on the stack
    pushq %rdi
    pushq %rdx

    # Perform some prep work to put the kernel in a good state
    call low_level_prep
    movq %rax, %cr3

    # Copy the memory map
    popq %rdi
    callq copy_memory_map

    # Setup our GDT
    lea gdtr(%rip), %rax
    addq $2, %rax
    lea gdt64(%rip), %rbx
    movq %rbx, (%rax)
    subq $2, %rax
    lgdt (%rax)

    # Set CS
    movq %rsp, %rax
    pushq $0x10
    pushq %rax  # Original %rsp
    pushfq
    pushq $0x08
    lea _set_cs(%rip), %rax
    pushq %rax
    iretq

_set_cs:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %gs
    movw %ax, %fs

    # Compute higher-half address
    movq $0xFFFFFFFF80000000, %rax
    lea _high_entry64(%rip), %rbx
    popq %rdi
    subq %rdi, %rbx
    addq %rbx, %rax

    # Jump into the higher-half
    jmpq *%rax

_high_entry64:
    # Unmap lower half
    movq %cr3, %rax
    movq $0, (%rax)
    movq %rax, %cr3

    # Fix stack
    movq $0xFFFFFFFFFFFFFFFF, %rsp
    call kernel_main

    cli
    hlt

.section .data
gdt64:
    # Null Segment
    .quad 0

    # Kernel Code Segment
    .short 0xFFFF
    .short 0
    .byte 0
    .byte 0b10011000 # P + DPL0 + Exec
    .byte 0b10111111 # G + L + AVL + Limit
    .byte 0

    # Kernel Data Segment
    .short 0xFFFF
    .short 0
    .byte 0
    .byte 0b10010010 # P + DPL0 + R/W
    .byte 0b10111111 # G + L + AVL + Limit
    .byte 0

gdtr:
    .short 24
    .quad 0    # Determined at runtime
