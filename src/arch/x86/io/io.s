.code32

.section .text

.global insb
.global insw
.global outb
.global outw

insb:
	push %ebp
	mov %esp, %ebp

	xor %eax, %eax
	movl 8(%ebp), %edx
	in %dx, %al
	
	pop %ebp
	ret

insw:
	push %ebp
	mov %esp, %ebp

	xor %eax, %eax
	mov 8(%ebp), %edx
	in %dx, %ax

	pop %ebp
	ret

outb:
	pushl %ebp
	movl %esp, %ebp

	movl 12(%ebp), %eax
	movl 8(%ebp), %edx
	outb %al, %dx

	popl %ebp
	ret

outw:
	push %ebp
	mov %esp, %ebp
	
	mov 12(%ebp), %eax
	mov 8(%ebp), %edx
	out %ax, %dx

	pop %ebp
	ret
