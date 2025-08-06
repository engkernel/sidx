#include "gdt.h"
#include "task/tss.h"
#include "mm/mm.h"
#include "utils/log.h"
#include <stdint.h>

struct tss tss;
struct segment_descriptor gdt[GDT_ENTRY_COUNT];

void gdt_set_segment(struct segment_descriptor* seg, uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	if (index >= GDT_ENTRY_COUNT || index < 0) 
	{
		kpanic("index value %d is not in range gdt entries %d",
				index, GDT_ENTRY_COUNT);
	}

	gdt[index].limit_low = (uint16_t)(limit & 0xffff);
	gdt[index].base_low = (uint16_t)(base & 0xffff);
	gdt[index].base_middle = (uint8_t)((base >> 16) & 0xff);	
	gdt[index].access = access;
	gdt[index].flags_limit = 
					(uint8_t)(((limit >> 16) & 0x0f) | (granularity & 0xf0));
					
	gdt[index].base_high = (uint8_t)((base >> 24) & 0xff);
}

extern void gdt_load(uint16_t limit, uint32_t base);

void gdt_init()
{
	kinfo("initialize GDT!");
	
	/* set up gdt segments */
	/* null entry */
	gdt_set_segment(gdt, GDT_NULL, 0, 0, 0, 0);

	/* kernel code */
	gdt_set_segment(gdt, GDT_KERNEL_CS, 0x00000000, 0xffffffff, GDT_KERNEL_CS_FLAGS, GDT_GRANULARITY);

	/* kernel data */
	gdt_set_segment(gdt, GDT_KERNEL_DS, 0x00000000, 0xffffffff, GDT_KERNEL_DS_FLAGS, GDT_GRANULARITY);

	/* user code */
	gdt_set_segment(gdt, GDT_USER_CS, 0x00000000, 0xffffffff, GDT_USER_CS_FLAGS, GDT_GRANULARITY);

	gdt_set_segment(gdt, GDT_USER_DS, 0x00000000, 0xffffffff, GDT_USER_CS_FLAGS, GDT_GRANULARITY);
	/* user data*/
	/* tss */
	memset(&tss, 0, sizeof(tss));
	tss.ss0 = GDT_KERNEL_DS << 3;
	tss.esp0 = 0x60000;
	gdt_set_segment(gdt, GDT_TSS, (uint32_t)&tss, sizeof(tss)-1, GDT_FLAG_TSS, GDT_GRANULARITY_NO);

	uint16_t gdt_limit = sizeof(gdt) - 1;
	uint32_t gdt_base = (uint32_t)&gdt;

	gdt_load(gdt_limit, gdt_base);
	tss_load(GDT_TSS << 3);
}