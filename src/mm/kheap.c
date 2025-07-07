#include "kheap.h"
#include "buddy.h"
#include "mm.h"

void* kmalloc(size_t size)
{
	return buddy_alloc(size_to_order(size));
}

void kfree(void* addr)
{
	int order = addr_to_order(addr);
	buddy_free(addr, order);
}

void* kzalloc(size_t size)
{
	void* ptr = kmalloc(size);
	if (!ptr)
		return 0;

	memset(ptr, 0x00, size);
	return ptr;
}


