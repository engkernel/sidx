#ifndef GDT_H
#define GDT_H

#define TOTAL_GDT_ENTRIES 6

#include "task/tss.h"


struct segment_descriptor {
	uint16_t first_limit;
	uint16_t first_base;
	uint8_t second_base;
	uint8_t access_byte;
	uint8_t limit_flags;
	uint16_t second_limit;
} __attribute__((packed));

struct gdt {
	uint32_t base;
	uint32_t limit;
	uint8_t access_byte;
	uint8_t flags;
};

void encode_gdt_enrty(uint8_t *target, struct gdt source);
void load_gdt(struct segment_descriptor* segment_desc, int size);
void gdt_init(struct gdt* gdt, struct segment_descriptor* segment_desc, struct tss* tss);

#endif