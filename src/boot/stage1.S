.code16

.global _start

.set KERNEL_CS, 0x8
.set KERNEL_DS, 0x10

_start:
	/* intel docs page 3353 */
	cli
	xor %ax, %ax
	movw %ax, %dx
	movw %ax, %es
	movw %ax, %ss
	movw $0x7c00, %sp /* we do not use sp */
	
	/* load gdtr */
	lgdt gdtr

	/* enable protected mode */
	movl %cr0, %eax
	xorl $0x1, %eax
	movl %eax, %cr0

	/* long jump, switches to protected mode */
	ljmp $KERNEL_CS, $protected_mode
		
/* gdt table */
gdt:
null:
	.quad 0x0000000000000000	/* NULL */ 
	.quad 0x00cf9b000000ffff	/* KERNEL_CS */ 
	.quad 0x00cf93000000ffff 	/* KERNEL_DS */
gdt_end:
gdtr:
	.word gdt_end - gdt - 1   	/* gdtr limit - 1 */
	.long gdt			/* gdtr base */

.code32
protected_mode:
	hlt

.org 510
	.word 0xaa55
