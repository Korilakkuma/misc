_main:                                  ## @main
	movl	$0, -4(%rsp)
	movl	$527, -8(%rsp)                  ## imm = 0x20F
	movl	-8(%rsp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rsp)
	movl	$322, -12(%rsp)                 ## imm = 0x142
	movl	-8(%rsp), %eax
	addl	-12(%rsp), %eax
	movl	%eax, -12(%rsp)
	movl	-12(%rsp), %eax
	retq
