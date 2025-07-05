.code32
.section .text
.global _start
.global kmain

.set CODE_SEG, 0x08
.set DATA_SEG, 0x10

_start:
	mov $DATA_SEG, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $0x00200000, %ebp
	mov %ebp, %esp
	
	# enable A20 line
	in $0x92, %al
	or $2, %al
	outb %al, $0x92
	
	# Remap the master PIC
	movb $0x11, %al
	outb %al, $0x20

	movb $0x20, %al
	outb %al, $0x21

	movb $0x01, %al
	outb %al, $0x21

	call kmain
loop:
	jmp loop

.fill 512 - (. - _start), 1, 0
	
