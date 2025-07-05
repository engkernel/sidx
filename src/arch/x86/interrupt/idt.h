#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define TOTAL_INTERRUPTS 512

struct idtr_desc
{
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct intr_desc
{
	uint16_t offset_1;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attribute;
	uint16_t offset_2;
}__attribute__((packed));

void idt_init();
void enable_interrupt();
void disable_interrupt();
#endif
