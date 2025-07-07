.code32
.section .text
	
.global enable_paging
.global load_page_directory

enable_paging:
	push %ebp
	mov %esp, %ebp

	mov %cr0, %eax
	or 0x80000000, %eax
	mov %eax, %cr0

	mov %ebp, %esp
	pop %ebp
	ret	

load_page_directory:
	push %ebp
	mov %esp, %ebp

	mov 8(%ebp), %eax
	mov %eax, %cr3

	mov %esp, %esp
	pop %ebp
	ret
