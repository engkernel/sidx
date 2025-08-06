.code32
.section .text
.global tss_load

# it does not reset the kernel but not sure if its correct
# need to implement userspace to check
tss_load:
    movw 4(%esp), %ax
    ltr %ax

    ret
