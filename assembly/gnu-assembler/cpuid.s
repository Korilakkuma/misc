	.global _start
	.text
_start:
	movl $1, %eax
	cpuid
	movl %edx, %ebx
	shrl $24, %ebx  # >> 24
	movl $1, %eax  # `exit` system call
	int $0x80
