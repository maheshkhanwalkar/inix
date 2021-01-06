#pragma once

typedef struct regs {
    /* Saved by _irq_common_base() */
    unsigned long ds;
    unsigned long r15, r14, r13, r12, r11, r10, r9, r8;
    unsigned long rbp, rdi, rsi, rdx, rcx, rbx, rax;
    unsigned long int_no;
    /* Provided by the CPU */
    unsigned long err_code;
    unsigned long rip, cs, rflags, rsp, ss;
} regs_t;
