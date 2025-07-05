#include "pmm.h"
#include "buddy.h"
#include "mm.h"
#include "arch/x86/mmap/mmap.h"
#include "utils/log.h"

void pmm_init()
{
	struct e820_entry* entry = 0;
	kinfo("pmm initialize!");
	init_phys_regions(entry);	
	/*
	if (!entry)
	{
		kerr("no physical regions");
		return;
	}
	kinfo("base: 0x%lx, len: 0x%lx", entry->base, entry->len);
	*/
	buddy_init(BUDDY_POOL_ADDRESS, BUDDY_POOL_SIZE);
	void* page = buddy_alloc(10);
	void* bloc = buddy_alloc(10);
	if (page)
		kinfo("page 0x%x", page);
	if (bloc)
		kinfo("block 0x%x", bloc);
	
	buddy_free(page, 4);
	buddy_free(page, 2);
}
