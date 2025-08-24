#ifndef ARCH_H
#define ARCH_H

#include <stddef.h>

/* arch */
void arch_init();

/* vga */
void vga_init();
void vga_put(int level, char c);
void vga_puts(int level, char* str);
void vga_new_line();
void vga_tab();
void cls();

/* memory */
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif /* ARCH_H */
