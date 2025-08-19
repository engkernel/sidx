#ifndef ARCH_x86_H
#define ARCH_x86_H

#include <stdint.h>

/* vga */
#define VGA_TEXT_MEM 0xB8000
void vga_init();

/* io */
static inline void outb(uint16_t port, uint8_t val)
{
	__asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

static inline void io_wait(void)
{
	outb(0x80, 0);
}

/* gdt */
void gdt_init();
extern void gdt_flush(uint32_t);

/* interrupts */
void traps_init();

static inline void enable_interrupts()
{
	__asm__ volatile("sti");
}

static inline void disable_interrupts()
{
	__asm__ volatile("cli");
}

/* mmu */
/* e820 memory maps */
void e820_init();

/* */
#endif
