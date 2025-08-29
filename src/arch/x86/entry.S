.code32
.section .text.boot
.global _start
.extern kmain

.set KERNEL_DS, 0x10
.set HIGH_KERNEL_BASE, 0xc0000000
.set TEMP_STACK, 0x0020000

_start:
	mov $KERNEL_DS, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $0x00200000, %ebp
	mov %ebp, %esp

	/* setup page table and enable paging */
	call setup_early_paging

	mov $HIGH_KERNEL_BASE + 0x10000, %esp
	mov %esp, %ebp

	jmp HIGH_KERNEL_BASE + kmain
loop:
	jmp loop

.fill 512-(.-_start),1,0
