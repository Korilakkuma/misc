	.global _start
	.text
_start:
	movl $12, %eax
	addl $14, %eax

	movl %eax, %ebx
	movl $1, %eax
	int $0x80
