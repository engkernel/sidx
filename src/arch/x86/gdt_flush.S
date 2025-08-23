.code32
.section .text

.global gdt_flush
.type gdt_flush, @function

gdt_flush:
	mov 4(%esp), %eax
	lgdt (%eax)	/* load gdtr from pointer in eax */

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ljmp $0x08, $1f /* to reload CS we need a far jump (0x08 kernel cs) */
1:
	ret
.size gdt_flush, .-gdt_flush
