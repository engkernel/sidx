#include "arch.h"
#include "include/log.h"

/* ====================== Definitions ===================== */
/* its a hardcode address used in real mode already */
#define E820_TOTAL_ENTRIES 0x8000
#define E820_ENTRIES_BUFFER 0x8004

struct e820_entry {
    uint64_t base;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));

/* ===================== Functions ========================*/
void e820_init()
{
	uint32_t e820_total = *(volatile uint32_t*) E820_TOTAL_ENTRIES;
	struct e820_entry *entries = (struct e820_entry*) E820_ENTRIES_BUFFER;
	for (int i = 0; i < e820_total; i++)
	{
			kinfo("base %x, len %x, type %d", entries[i].base, entries[i].len, (uint32_t)entries[i].type);
	}
}

