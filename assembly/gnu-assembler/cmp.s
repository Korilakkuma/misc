	.global _start
	.text
_start:
	movl $512, %eax
	cmpl $456, %eax
	je L1
	movl $msg_neq, %ecx
	movl $10, %edx
	jmp L2
L1:
	movl $msg_eq, %ecx
	movl $6, %edx
L2:
	movl $4, %eax  # `write` system call
	movl $1, %ebx  # stdout
	int $0x80

	movl $1, %eax # `exit` system call
	int $0x80

	.data

msg_eq: .ascii "equal\n"
msg_neq: .ascii "not equal\n"
