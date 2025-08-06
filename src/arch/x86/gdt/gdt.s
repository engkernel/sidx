.code32
.section .text

.global gdt_load
.type gdt_load, @function

gdt_load:
    mov 4(%esp), %eax
    mov %eax, gdt_desc+2

    mov 8(%esp), %ax
    mov %ax, gdt_desc

    lgdt gdt_desc
    ret

.section .data
.align 8
gdt_desc:
    .word 0x0000
    .long 0x00000000
