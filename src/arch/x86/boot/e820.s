
.code16
.section .text
.global get_e820_mmap
.type get_e820_mmap @function

.set mmap_ent, 0x8000

get_e820_mmap:
	movw $0x8004, %di
	xor %ebx, %ebx
	xor %bp, %bp
	mov $0x0534D4150, %edx	
	mov $0xE820, %eax
	movw $1, %es:20(%di)
	mov $24, %ecx
	int $0x15
	jc .failed
	mov $0x0534D4150, %edx
	cmp %edx, %eax
	jne .failed
	test %ebx, %ebx
	je .failed
	jmp .jmpin
	
.e820lp:
	mov $0xE820, %eax
	movw $1, %es:20(%di)
	mov $24, %ecx
	int $0x15
	jc .e820f
	mov $0x0534D4150, %edx
.jmpin:
	jcxz .skipent
	cmp $20, %cl
	jbe .notext
	testw $1, %es:20(%di)
	je .skipent	
.notext:
	mov %es:8(%di), %ecx
	or %es:12(%di), %ecx
	jz .skipent
	inc %bp
	add $24, %di
.skipent:
	test %ebx, %ebx
	jne .e820lp
.e820f:
	movw %bp, %es:mmap_ent
	clc
	ret
.failed:
	stc
	ret

