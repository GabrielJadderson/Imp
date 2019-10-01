.data
.align 8
heap_pointer: 
.space 16394
heap_next: 
.quad 0

.bss
.section	.rodata
printf_format_string: .asciz "%s\n"
printf_format_int: .asciz "%d\n"
printf_format_nl: .asciz "\n"

.globl  main

.text
main:
pushq	%rbp
movq	%rsp, %rbp
pushq	%rbx
pushq	%r12
pushq	%r13
pushq	%r14
pushq	%r15
subq	$48, %rsp	#make space for variables
movq	$2, -8(%rbp)	#Assignment statement.
movq	$4, -16(%rbp)	#Assignment statement.
movq	$10, -24(%rbp)	#Assignment statement.
movq	-8(%rbp), %rdx	#expression left
movq	-16(%rbp), %rax	#expression right
imulq	%rdx, %rax	#expression calculation
movq	-24(%rbp), %rdx	#expression right
addq	%rdx, %rax	#expression calculation

movq	%rax, -32(%rbp)	#Assignment statement.
movq	$printf_format_int, %rdi
movq	-32(%rbp), %rsi
movq	$0, %rax
xor	%eax, %eax	#Zeroing EAX is efficient way to clear AL and set it to 0.
call	printf
popq	%r15
popq	%r14
popq	%r13
popq	%r12
popq	%rbx
movq	%rbp, %rsp
popq	%rbp
retq
