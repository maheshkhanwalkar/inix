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
