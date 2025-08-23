#include "arch.h"
#include "include/string.h"

#include <stddef.h>

/* =================== Definitions ====================*/
/* PIC 
 * support legacy */

/* master PIC */
#define PIC1 0x20
/* slave PIC */
#define PIC2 0xa0
#define PIC1_CMD 	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_CMD	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20
/* remap PIC */
#define ICW1_ICW4	0x01 	/* ICW4 will be present */
#define ICW1_SINGLE	0x02	/* cascade mode */
#define ICW1_INTERVAL4	0x04	/* call address interval 4(8) */
#define ICW1_LEVEL	0x08  	/* level triggered */
#define ICW1_INIT 	0x10	/* init required */

#define ICW4_8086	0x01	/* 8086/88 mode */
#define ICW4_AUTO	0x02	/* auto EOI */
#define	ICW4_BUF_SLAVE	0x08	/* buffered slave */
#define ICW4_BUF_MASTER	0x0C	/* buffered master */
#define ICW4_SFNM	0x10	/* special fully nested (not) */

#define CASCADE_IRQ 	2

#define PIC_OFFSET_VEC	0x20 	/* replace the PIC offset */
/* interrupt descriptor */
struct idt_entry {
	uint16_t low_offset;
	uint16_t selector;
	uint8_t zero; /* unused, for padding */
	uint8_t type_attributes;
	uint16_t high_offset;
}__attribute__((packed));

#define TOTAL_INTERRUPTS 256
struct idt {
	uint16_t size;
	uint32_t base;
}__attribute__((packed));

/* live in whole state of kernel */
struct idt_entry idt_entries[TOTAL_INTERRUPTS];
struct idt idtr;

/* kernel cs segment */
#define KERNEL_CS	0x08

/* ======================== Prototypes ====================*/

/* interrupts */
extern void empty_isr();

/* ======================== Functions =====================*/

/* send end of interrupt */
void pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
		outb(PIC2_CMD, PIC_EOI);
	outb(PIC1_CMD, PIC_EOI);
}

/* @args
 * offset1 - vector offset for master
 */
void pic_remap(int offset)
{
	/* start init sequence in cascade mode */
	outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
	io_wait();
	/* ICW2: master PIC vector offset */
	outb(PIC1_DATA, offset);
	io_wait();
	/* ICW2: slave PIC vector offset, we choose master+8 */
	outb(PIC2_DATA, offset+8);	
	io_wait();

	/* ICW3: tell master PIC that htere is a slave at IRQ2 */
	outb(PIC1_DATA, 1 << CASCADE_IRQ);
	io_wait();

	/* ICW3: tell slave PIC its cascade identity (0000 0010) */
	outb(PIC2_DATA, 2);
	io_wait();

	/* ICW4: have the PICs use 8086 mode (and not 8080 mode) */
	outb(PIC1_DATA, ICW4_8086);
	io_wait();

	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	/* unmask both PICs */
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

/* PIC disable */
void pic_disable(void) 
{
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}

/* isr handlers */
void empty_handler()
{
	/* just do nothing and send EOI */
	outb(0x20, 0x20);
}

static inline void idt_set_entry(struct idt_entry *entry, uint16_t handler)
{
	entry->low_offset = (uint16_t)(handler & 0xffff);
	entry->selector = KERNEL_CS;
	entry->zero = 0x00;
	entry->type_attributes = 0x8E; /* interrupt gate */
	entry->high_offset = (uint16_t)(handler >> 16);

}

/* traps initialize */
void traps_init()
{
	const uint32_t addr = (uint32_t) empty_isr;

	/* remap PIC */
	pic_remap(PIC_OFFSET_VEC);

	/* init idt desc and set handler interrupts */
	memset(idt_entries, 0, sizeof(idt_entries));
	idtr.size = (uint16_t)(sizeof(idt_entries) - 1);
	idtr.base = (uint32_t)idt_entries;

	/* we override all interrupts handlers intentionally */
	for (int i = 0; i < TOTAL_INTERRUPTS; i++)
	{
		idt_set_entry(&idt_entries[i], addr);
	}
	/* lidt */
	__asm__ ("lidt %0" : : "m"(idtr));
}
