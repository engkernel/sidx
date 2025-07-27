.code32
.section .text
.global tss_load

# it does not reset the kernel but not sure if its correct
# need to implement userspace to check
tss_load:
    mov 8(%esp), %eax
    ltr %ax

    ret
