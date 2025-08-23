/* wrapper for all interrupt handlers */
.code32
.section .text

.extern empty_handler

.global empty_isr
.type empty_isr, @function

empty_isr:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %ebx
	pushl %esp
	pushl %ebp
	pushl %esi
	pushl %edi
	cld
	call empty_isr
	popl %edi
	popl %esi
	popl %ebp
	popl %esp
	popl %ebx
	popl %edx
	popl %ecx
	popl %eax
	iret
