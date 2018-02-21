	.file	"lll3.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Done! %f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	xorpd	%xmm2, %xmm2
	movl	$100000, %esi
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80016, %rsp
	movq	%rsp, %rcx
	subq	$80016, %rsp
	movq	%rsp, %rdx
	.p2align 4,,10
	.p2align 3
.L2:
	movapd	%xmm2, %xmm0
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L5:
	movsd	(%rcx,%rax), %xmm1
	mulsd	(%rdx,%rax), %xmm1
	addq	$8, %rax
	cmpq	$80000, %rax
	addsd	%xmm1, %xmm0
	jne	.L5
	subl	$1, %esi
	jne	.L2
	movl	$.LC1, %edi
	movl	$1, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
