#ifndef x86_MMAP
#define x86_MMAP

#include <stdint.h>

#define MEM_AVAILABLE 	1
#define MEM_RESERVED 	2
#define MEM_ACPI_RECL	3
#define MEM_NVS		4
#define MEM_BAD_RAM	5

#define E820_MAP_ADDRESS 0x8004
#define E820_COUNT_PTR	(*(uint32_t*)0x8000)
#define E820_MAP_BASE	((struct e820_entry*)E820_MAP_ADDRESS)

/* we use for now only two regions as we do not need much */
#define TOTAL_AVAILABLE_REGIONS 2

/* we read it from src/linker.ld kernel_phys_end as last section */
extern uint32_t kernel_end_phys;

#define KERNEL_END_PHYS ((uint32_t)&kernel_end_phys)

struct e820_entry
{
	uint64_t base;
	uint64_t len;
	uint32_t type; // TODO type&acpi should convert uint32_t to 20 byte value
	uint32_t acpi;
} __attribute__((packed));

void init_phys_regions();
void print_all_e820_regions();
struct e820_entry* e820_memory_map(uint32_t* count);
void e820_default_region(struct e820_entry* def);

#endif
