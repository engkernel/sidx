#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/* segment type */
#define GDT_TYPE_CODE_RX	0x0A 
#define GDT_TYPE_DATA_RW	0x02 

/* P DPL S */
#define GDT_FLAG_PRESENT		0x80
#define GDT_FLAG_NOT_PRESENT 	0x00

#define GDT_FLAG_RING0		0x00
#define GDT_FLAG_RING1		0x20
#define GDT_FLAG_RING2		0x40
#define GDT_FLAG_RING3		0x80

#define GDT_FLAG_SYSTEM			0x00
#define GDT_FLAG_CODEDATA 		0x01
#define GDT_FLAG_TSS 			0x89

/* G D/B L AVL */
#define GDT_FLAG_GRANULARITY_1B 0x00
#define GDT_FLAG_GRANULARITY_4K 0x80

#define GDT_FLAG_32BIT		0x40
#define GDT_FLAG_16BIT		0x00

#define GDT_FLAG_LONG_MODE_OFF	0x00
#define GDT_FLAG_LONG_MODE 		0x20

#define GDT_FLAG_AVAILABLE		0x10
#define GDT_FLAG_NOT_AVAILABLE 	0x00


#define GDT_KERNEL_CS_FLAGS \
	(GDT_FLAG_PRESENT | GDT_FLAG_RING0 | GDT_FLAG_CODEDATA | GDT_TYPE_CODE_RX)

#define GDT_KERNEL_DS_FLAGS \
	(GDT_FLAG_PRESENT | GDT_FLAG_RING0 | GDT_FLAG_CODEDATA | GDT_TYPE_DATA_RW)

#define GDT_USER_CS_FLAGS \
	(GDT_FLAG_PRESENT | GDT_FLAG_RING3 | GDT_FLAG_CODEDATA | GDT_TYPE_CODE_RX)

#define GDT_USER_DS_FLAGS \
	(GDT_FLAG_PRESENT | GDT_FLAG_RING3 | GDT_FLAG_CODEDATA | GDT_TYPE_DATA_RW)

#define GDT_GRANULARITY \
	(GDT_FLAG_GRANULARITY_4K | GDT_FLAG_32BIT)

#define GDT_GRANULARITY_NO 0

/* gdt entries */
#define GDT_ENTRY_COUNT 6
#define GDT_NULL		0
#define GDT_KERNEL_CS	1
#define GDT_KERNEL_DS	2
#define GDT_USER_CS		3
#define GDT_USER_DS		4
#define GDT_TSS			5


struct segment_descriptor {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access; 		// (P, DPL, S, TYPE)
	uint8_t flags_limit;
	uint8_t base_high;
}__attribute__((packed));

struct gdtr_register {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

extern struct segment_descriptor gdt[GDT_ENTRY_COUNT];

void gdt_set_segment(struct segment_descriptor* seg, uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

void gdt_init();

#endif