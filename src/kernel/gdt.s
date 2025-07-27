.code32
.section .text

.global load_gdt
.type gdt_load, @function

load_gdt:
    mov 4(%esp), %eax
    mov %eax, gdt_desc+2
    mov 8(%esp), %ax
    mov %ax, gdt_desc
    lgdt gdt_desc
    ret

.section .data
gdt_desc:
    .word 0x0000
    .long 0x00000000
