.code32
.section .text

.global tss_load

tss_load:
	push %ebp
	mov %esp, %ebp

	mov %ax, [%ebp+8]
	ltr %ax

	pop %ebp
	ret
