.code32
.section .text

.extern empty_handler
.extern keyboard_handler

.global isr_empty
.global isr_keyboard

isr_empty:
	cli
	pushal
	call empty_handler
	popal
	sti
	iret

isr_keyboard:
	cli
	pushal
	call keyboard_handler
	popal
	sti
	iret