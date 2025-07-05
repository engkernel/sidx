#include "buddy.h"
#include "mm.h"
#include "utils/log.h"

#define MAX_PAGES (1 << MAX_ORDER)
#define PAGE_IS_FREE 0x00
#define PAGE_IS_TAKEN 0x01

typedef struct page 
{
	struct page* next;
	uint8_t order;
	uint8_t is_free;
} page_t;

static page_t* free_lists[MAX_ORDER + 1];
static page_t* page_array = NULL;
static uintptr_t mem_base;

void buddy_init(uintptr_t base, size_t size)
{
	mem_base = base;
	size_t num_pages = size / PAGE_SIZE;

	page_array = (page_t*)mem_base;
	memset(page_array, 0, num_pages * sizeof(page_t));
	
	// set region
	size_t offset = 0;
	while(offset < num_pages)
	{
		for ( int order = MAX_ORDER; order >= 0; order --)
		{
			size_t block_size = 1 << order;
			if ((offset & (block_size -1)) != 0)
				continue;

			if (offset + block_size > num_pages)
				continue;

			page_t* page = &page_array[offset];
			page->order = order;
			page->is_free = PAGE_IS_FREE;
			page->next = free_lists[order];
			free_lists[order] = page;

			offset += block_size;
			break;
		}
	}
	kinfo("buddy initialized!");
}

static void remove_from_free_list(page_t** head, page_t* page)
{
	if (*head == page) 
	{
		*head = page->next;
		return;
	}
	page_t* prev = *head;
	while(prev && prev->next != page)
		prev = prev->next;
	
	if (prev)
		prev->next = page->next;
}

void* buddy_alloc(uint8_t order)
{
	for (int o = order; o <= MAX_ORDER; o--)
	{
		if (free_lists[o])
		{
			page_t* page = free_lists[o];
			remove_from_free_list(&free_lists[o], page);

			while (o > order)
			{
				o--;
				page_t* buddy = &page_array[page - page_array + (1 << o)];
				buddy->order = o;
				buddy->is_free = PAGE_IS_TAKEN;
				buddy->next = free_lists[o];
				free_lists[o] = buddy;
			}

			page->order = order;
			page->is_free = PAGE_IS_TAKEN;
			return (void*)(mem_base + ((page - page_array) * PAGE_SIZE));
		}
	}
	return NULL;
}

void buddy_free(void* ptr, uint8_t order)
{
	size_t index = (((uintptr_t)ptr - mem_base) / PAGE_SIZE);
	page_t* page = &page_array[index];
	page->order = order;
	page->is_free = PAGE_IS_FREE;
	
	while(order < MAX_ORDER)
	{
		size_t buddy_index = index ^ (1 << order);
		page_t* buddy = &page_array[buddy_index];

		if (buddy->is_free || buddy->order != order)
			break;

		remove_from_free_list(&free_lists[order], buddy);

		// Merge
		if (buddy_index < index)
			page = buddy;

		index = (index < buddy_index) ? index : buddy_index;

		page->order = ++order;
	}

	page->next = free_lists[index];
	free_lists[order] = page;
}
