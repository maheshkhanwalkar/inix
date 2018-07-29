.code32
.section .bstrapL,"ax",@progbits

.globl _setup_paging
.globl _enable_paging

_setup_paging:

    # Map 0x0000000000000000 -> 0x0000000000000000
    # Map 0xFFFFFFFF80000000 -> 0x0000000000000000

    movl pdL, %eax
    orl $0, %eax    # 0x0 address
    orl $0b10000011, %eax # P + R/W + PS

    movl %eax, pdL
    movl %eax, pdH

    movl $pdL, %eax
    orl $1, %eax    # P bit
    movl %eax, pdptL

    movl $pdH, %eax
    orl $1, %eax    # P bit

    movl $pdptH, %ebx
    addl $0xff0, %ebx
    movl %eax, (%ebx)

    movl $pdptL, %eax
    orl $0b11, %eax     # P + R/W
    movl %eax, pml4

    movl $pdptH, %eax
    orl $0b11, %eax     # P + R/W

    movl $pml4, %ebx
    addl $0xff8, %ebx
    movl %eax, (%ebx)

    ret

_enable_paging:

    # CR3 = PML4
    movl $pml4, %eax
    movl %eax, %cr3

    # CR4.PAE = 1
    movl %cr4, %eax
    orl $0x20, %eax
    movl %eax, %cr4

    # IA32_EFER.LME = 1
    movl $0xC0000080, %ecx
    rdmsr
    orl $0x100, %eax
    wrmsr

    # CR0.PG = 1
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    ret

.section .paging,"aw",@progbits

.globl pml4
.globl pdptL
.globl pdptH
.globl pdL
.globl pdH

pml4: .skip 4096
pdptL: .skip 4096
pdptH: .skip 4096
pdL: .skip 4096
pdH: .skip 4096

