
.section .text,"ax",@progbits
.globl spinlock_acquire
.globl spinlock_release

.do_spin:
	pause

spinlock_acquire:
	movq $1, %r11
	movq $0, %rax

	lock cmpxchgq %r11, (%rdi)
	jnz .do_spin

	retq

spinlock_release:
	movq $0, (%rdi)
	retq
