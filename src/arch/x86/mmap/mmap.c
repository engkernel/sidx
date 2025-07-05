#include "mmap.h"
#include "utils/log.h"
#include "utils/string.h"
#include <stdint.h>
#include <stddef.h>

/* TODO we need linked list functionalities */
static struct e820_entry e820_regions[TOTAL_AVAILABLE_REGIONS];

void init_phys_regions()
{
	uint16_t entry_count = E820_COUNT_PTR;
	const struct e820_entry* entries = E820_MAP_BASE;
	int num = 0;

	for (int i = 0; i < entry_count; i++)
	{
		if (entries[i].type == MEM_AVAILABLE)
		{
			e820_regions[num] = entries[i];
			num++;
		}
	}
}

void print_all_e820_regions()
{	
	uint16_t entry_count = E820_COUNT_PTR;
	const struct e820_entry* entries = E820_MAP_BASE;
	const char* region_type_str[] = 
	{
		"MEM AVAILABLE",
		"MEM RESERVED",
		"MEM ACPI RECLAIM",
		"MEM BAD RAM"
	};

	for (int i = 0; i < entry_count; i++)
	{
		const struct e820_entry* region = &entries[i];
		kinfo("%s base: 0x%lx, len: 0x%lx, type: %u, acpi: %u",
			region_type_str[region->type - 1], 
			region->base, region->len, 
			(region->type & 0xFFFFF),
			region->acpi);
		if (region->type == 1)
		{
			kinfo("Free MEM Size: %umb",  region->len  / (1024 * 1024));
		}
	}
}

static struct e820_entry* get_e820_regions(uint32_t* count)
{
	*count = TOTAL_AVAILABLE_REGIONS;
	return e820_regions;
}

struct e820_entry* e820_memory_map(uint32_t* count)
{
	/* TODO its not dynamic, I did not fixed receiving regions */
	return get_e820_regions(count);
}

void e820_default_region(struct e820_entry* def)
{ 
	/* TODO we check regions here, not effecient but for now its ok */
	for (int i = 0; i < TOTAL_AVAILABLE_REGIONS; i++)
	{
		const struct e820_entry* region = &e820_regions[i];
		uint64_t start = region->base;
		uint64_t end = start + region->len;
		
		if (end < KERNEL_END_PHYS)
			continue;
		
		if (start < KERNEL_END_PHYS)
		{
			start = KERNEL_END_PHYS;		
			def->base = start;
			def->len = region->len;
			kinfo("default memory base: 0x%lx, len: 0x%lx", def->base, def->len);
		}
	}
}

