.section .bstrapH,"ax",@progbits
.globl _enable_sse

_enable_sse:
    # Set OSFXSR and OSXMMEXCPT flags
    movq %cr4, %rax
    orq $0x600, %rax
    movq %rax, %cr4

    # Disable x87 FPU emulation
    movq %cr0, %rax
    movq $2, %rbx
    notq %rbx
    andq %rbx, %rax
    movq %rax, %cr0

    # Set CR0.MP = 1
    movq %cr0, %rax
    orq $0x2, %rax
    movq %rax, %cr0

    # Enable all SSE exception masks
    pushq $0x1f80
    ldmxcsr (%rsp)
    popq %rax

    retq