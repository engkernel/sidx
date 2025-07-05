#include "idt.h"
#include "isr.h"
#include "utils/log.h"
#include "mm/mm.h"
#include "arch/x86/config.h"

#include <stdint.h>

struct idtr_desc idtr_descriptor;
struct intr_desc intr_descriptors[TOTAL_INTERRUPTS];

extern void isr_empty();
extern void isr_keyboard();
extern void load_idt(struct idtr_desc* idtr);

void static set_intr_gate(int intr_no, void* handler)
{
	struct intr_desc* desc = &intr_descriptors[intr_no];
	desc->offset_1 = (uint32_t) handler & 0x0000FFFF;
	desc->selector = GDT_CODE_SEG;
	desc->zero = 0x00;
	desc->type_attribute = 0XEE;
	desc->offset_2 = (uint32_t) handler >> 16;
}

void static init_intr()
{
	for (int i = 0; i < TOTAL_INTERRUPTS; i++)
	{
		set_intr_gate(i, isr_empty);
	}	

	// Keyboard
	set_intr_gate(0x21, isr_keyboard);
}

void idt_init()
{
	kinfo("idt inititalize!");
	memset(intr_descriptors, 0, sizeof(intr_descriptors));
	idtr_descriptor.size = sizeof(intr_descriptors) - 1;
	idtr_descriptor.offset = (uint32_t)intr_descriptors;

	init_intr();
	load_idt(&idtr_descriptor);
}
