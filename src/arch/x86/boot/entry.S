.code32
.section .text
.global _start

.set KERNEL_DS, 0x10

_start:
	mov $KERNEL_DS, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $0x00200000, %ebp
	mov %ebp, %esp

	call kmain
loop:
	jmp loop

.fill 512-(.-_start),1,0
