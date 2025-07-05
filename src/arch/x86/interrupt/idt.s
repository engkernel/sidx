
.code32
.section .text

.global load_idt
.global enable_interrupt
.global disable_interrupt

load_idt:
	push %ebp
	mov %esp, %ebp
	
	mov 8(%ebp), %ebx
	lidt (%ebx) 

	pop %ebp
	ret

enable_interrupt:
	sti
	ret

disable_interrupt:
	cli
	ret
