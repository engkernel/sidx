#include "gdt.h"
#include "task/tss.h"
#include "mm/mm.h"
#include "utils/log.h"
#include <stdint.h>

void encode_gdt_entry(struct segment_descriptor desc, struct gdt source)
{
	if (source.limit > 0xFFFFF)
		kpanic("GDT cannot encode limits larger than 0xFFFFF");

	desc.first_limit = source.limit & 0xFFFF;
	desc.first_base = source.base & 0xFFFF;
	desc.second_base = (source.base >> 16) & 0xFF;
	desc.access_byte = source.access_byte;
	desc.limit_flags = ((source.limit >> 16) & 0x0F) | ((source.flags & 0x0F) << 4);
	desc.second_limit = (source.base >> 24) & 0xFF;  // 8 bits only!
}


void gdt_to_segment_desc(struct gdt* gdt, struct segment_descriptor* segment_desc, int total_entries) 
{
	for (int i = 0; i < total_entries; i++)
	{
		encode_gdt_entry(segment_desc[i], gdt[i]);
	}
}

void gdt_init(struct gdt* gdt, struct segment_descriptor* segment_desc, struct tss* tss)
{
	// memset(gdt, 0x00, sizeof(struct gdt) * TOTAL_GDT_ENTRIES);
	memset(segment_desc, 0x00, sizeof(struct segment_descriptor) * TOTAL_GDT_ENTRIES);
	
	// null segment
	gdt[0].base = 0x00;
	gdt[0].limit = 0x00;
	gdt[0].access_byte = 0x00;
	gdt[0].flags = 0x00;

	// kernel code segment
	gdt[1].base = 0x00;
	gdt[1].limit = 0xfffff;
	gdt[1].access_byte = 0x9a;
	gdt[1].flags = 0xc;

	// kernel data segment
	gdt[2].base = 0x00;
	gdt[2].limit = 0xfffff;
	gdt[2].access_byte = 0x92;
	gdt[2].flags = 0xc;

	// user code segment
	gdt[3].base = 0x00;
	gdt[3].limit = 0xfffff;
	gdt[3].access_byte = 0xfa;
	gdt[3].flags = 0xc;

	// user data segment
	gdt[4].base = 0x00;
	gdt[4].limit = 0xfffff;
	gdt[4].access_byte = 0xf2;
	gdt[4].flags = 0xc;

	// task state
	gdt[5].base = (uint32_t)tss;
	gdt[5].limit = sizeof(struct tss)-1;
	gdt[5].access_byte = 0x89;
	gdt[5].flags = 0x0;

	gdt_to_segment_desc(gdt, segment_desc, TOTAL_GDT_ENTRIES);

	// load new gdt	
	load_gdt(segment_desc, sizeof(struct segment_descriptor));
}