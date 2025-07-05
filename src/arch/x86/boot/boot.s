.code16
.global _start
.extern get_e820_mmap

.set STACK_ADDR,	0x7c00
.set CODE_SEG,		gdt_code - gdt_start
.set DATA_SEG,		gdt_data - gdt_start

_start:
	jmpl $0, $real_mode
	nop # make it 8 byte align
		
real_mode:
	cli
	xorw	%ax, %ax
	movw	%ax, %es
	movw	%ax, %ss
	movw 	%ax, %ds		 	
	movw 	$STACK_ADDR, %sp

	# get memory map
	call 	get_e820_mmap	

	lgdt 	gdt_desc
	movl 	%cr0, %eax
	orl	$1, %eax
	movl	%eax, %cr0

	jmpl	$CODE_SEG, $protected_mode 
# GDT 
.align 8

gdt_start:
gdt_null:
	.word 0,0,0,0 
gdt_code:
	.word 0xFFFF
	.word 0x0000
	.word 0x9A00
	.word 0x00CF
gdt_data:
	.word 0xFFFF
	.word 0x0000
	.word 0x9200
	.word 0x00CF
gdt_end:
gdt_desc:
	.word gdt_end - gdt_start - 1	
	.long gdt_start	

.code32
.set START_SECT, 1
.set TOTAL_SECT, 100
.set TOTAL_WORD, 256
.set DEST_ADDR, 0x0100000


protected_mode:
	mov $START_SECT, %eax
	mov $TOTAL_SECT, %ecx
	mov $DEST_ADDR, %edi	
	call read_disk
	jmpl $CODE_SEG, $DEST_ADDR

read_disk:
	mov %eax, %ebx
		
	mov $0x01F6, %dx
	shrl $24, %eax 
	or $0xE0, %eax
	out %al, %dx

	mov $0x01F2, %dx
	mov %cl, %al
	out %al, %dx

	mov $0x1F3, %dx
	mov %ebx, %eax
	out %al, %dx

	mov $0x1F4, %dx
	mov %ebx, %eax
	shrl $8, %eax
	out %al, %dx

	mov $0x1F5, %dx
	mov %ebx, %eax
	shrl $16, %eax
	out %al, %dx
	
	mov $0x1F7, %dx
	mov $0x20, %al
	out %al, %dx

next_sector:
	push %ecx
try_read:
	mov $0x1F7, %dx
	in %dx, %al
	test $8, %al
	jz try_read	
	
	mov $TOTAL_WORD, %ecx
	mov $0x1F0, %dx
	rep insw
	pop %ecx
	loop next_sector
	ret

.include "e820.s"	
.org 510
	.word 0xAA55
