	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	movl	$0, -4(%rsp)
	movl	_i(%rip), %eax
	addl	$712, %eax                      ## imm = 0x2C8
	movl	%eax, _i(%rip)
	xorl	%eax, %eax
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__DATA,__data
	.globl	_i                              ## @i
	.p2align	2
_i:
	.long	125                             ## 0x7d

.subsections_via_symbols
