#include "gdt.h"
#include "utils/error.h"
#include <stdint.h>

void encode_gdt_enrty(uint8_t *target, struct gdt source)
{
	if (source.limit > 0xfffff)
	{
		kpanic("GDT cannot encode limits larget than 0xfffff");
	}

	// encode limits
	target[0] = source.limit & 0xff;
	target[1] = (source.limit >> 8) & 0xff;
	target[6] = (source.limit >> 16) & 0x0f;

	// encode the base
	target[2] = source.base & 0xff;
	target[3] = (source.base >> 8) & 0xff;
	target[4] = (source.base >> 16) & 0xff;
	target[7] = (source.base >> 24) & 0xff;

	// encode the access byte
	target[5] = source.access_byte;

	// encode the flags
	target[6] |= (source.flags << 4);
}
