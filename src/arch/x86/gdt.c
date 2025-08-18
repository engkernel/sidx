#include "arch.h"
#include "include/string.h"

/* ==================== Definitions ===============*/
struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
}__attribute__((packed));

struct gdt {
	uint16_t size;
	uint32_t base;
}__attribute__((packed));

#define TOTAL_GDT	6

/* live total life of kernel */
struct gdt gdtr;
struct gdt_entry gdt_entries[TOTAL_GDT]; 

/* =====================Prototypes===================*/
extern void gdt_flush(uint32_t);

/*======================Functions===================*/
static void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	gdt_entries[index].base_low = (base & 0xffff);
	gdt_entries[index].base_middle = (base >> 16) & 0xff;
	gdt_entries[index].base_high = (base >> 24) & 0xff;

	gdt_entries[index].limit_low = (limit & 0xffff);
	gdt_entries[index].granularity = ((limit >> 16) & 0x0f);

	gdt_entries[index].granularity |= (granularity & 0xf0);
	gdt_entries[index].access = access;
}

void gdt_init()
{
	/* NULL descriptor */
	gdt_set_entry(0, 0, 0, 0, 0);

	/* kernel code: base 0, limit 4GiB, DPL=0, code rx, present, 32-bit, 4KiB gran*/
	gdt_set_entry(1, 0, 0xffffffff, 0x9a, 0xcf);

	/* kernel data: base 0, limit 4GiB, DPL=0, data rw, present, 32-bit, 4KiB gran*/
	gdt_set_entry(2, 0, 0xffffffff, 0x92, 0xcf);

	/* user code segment */
	gdt_set_entry(3, 0, 0xffffffff, 0xfa, 0xcf);

	/* user data segment */
	gdt_set_entry(4, 0, 0xffffffff, 0xf2, 0xcf);

	/* TSS will be set later when multitasking is implemented */
	gdt_set_entry(5, 0, 0, 0, 0);

	gdtr.size = sizeof(gdt_entries) - 1;
	gdtr.base = (uint32_t)gdt_entries;

	/* load gdt and reload cs (flush) */
	gdt_flush((uint32_t)&gdtr);
}
