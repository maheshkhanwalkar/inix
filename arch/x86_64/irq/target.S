.section .text

.globl __isr0, __isr1, __isr2, __isr3, __isr4, __isr5, __isr6
.globl __isr7, __isr8, __isr9, __isr10, __isr11, __isr12, __isr13
.globl __isr14, __isr15, __isr16, __isr17, __isr18, __isr19, __isr20
.globl __isr21, __isrReserved

__isr0:
    pushq $0
    pushq $0
    jmp _irq_common_base

__isr1:
    pushq $0
    pushq $1
    jmp _irq_common_base

__isr2:
    pushq $0
    pushq $2
    jmp _irq_common_base

__isr3:
    pushq $0
    pushq $3
    jmp _irq_common_base

__isr4:
    pushq $0
    pushq $4
    jmp _irq_common_base

__isr5:
    pushq $0
    pushq $5
    jmp _irq_common_base

__isr6:
    pushq $0
    pushq $6
    jmp _irq_common_base

__isr7:
    pushq $0
    pushq $7
    jmp _irq_common_base

__isr8:
    pushq $8
    jmp _irq_common_base

__isr9:
    pushq $0
    pushq $9
    jmp _irq_common_base

__isr10:
    pushq $10
    jmp _irq_common_base

__isr11:
    pushq $11
    jmp _irq_common_base

__isr12:
    pushq $12
    jmp _irq_common_base

__isr13:
    pushq $13
    jmp _irq_common_base

__isr14:
    pushq $14
    jmp _irq_common_base

__isr15:
    pushq $0
    pushq $15
    jmp _irq_common_base

__isr16:
    pushq $0
    pushq $16
    jmp _irq_common_base

__isr17:
    pushq $17
    jmp _irq_common_base

__isr18:
    pushq $0
    pushq $18
    jmp _irq_common_base

__isr19:
    pushq $0
    pushq $19
    jmp _irq_common_base

__isr20:
    pushq $0
    pushq $20
    jmp _irq_common_base

__isr21:
    pushq $21
    jmp _irq_common_base

__isrReserved:
    pushq $0
    pushq $22
    jmp _irq_common_base

_irq_common_base:
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    # Save previous data segment
    xorq %rax, %rax
    movw %ds, %ax
    pushq %rax

    # Setup kernel data segment
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    # Execute common handler
    movq %rsp, %rdi
    call irq_common_handler

    # Restore the previous segments
    popq %rax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    # Restore all the saved regs
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

    # Adjust for ISR number (pushed by __isrX)
    # Adjust for error code (pushed by CPU or __isrX)
    addq $16, %rsp
    iretq
