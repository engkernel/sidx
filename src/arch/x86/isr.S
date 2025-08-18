/* wrapper for all interrupt handlers */
.code32
.section .text

.extern empty_isr

.global isr_wrapper
.type isr_wrapper, @function

isr_wrapper:
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
