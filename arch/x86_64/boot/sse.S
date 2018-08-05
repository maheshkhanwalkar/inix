.section .bstrapH,"ax",@progbits
.globl _enable_sse

_enable_sse:
    # Set OSFXSR and OSXMMEXCPT flags
    movq %cr4, %rax
    orq $0x600, %rax
    movq %rax, %cr4

    # Enable IM, DM, ZM, OM, UM, and PM
    pushq $0x1f80
    ldmxcsr (%rsp)

    popq %rax
    retq