#include "arch.h"
#include "include/log.h"
#include "include/string.h"

#include <stddef.h>

/* ====================== Definitions ===================== */
extern char __KERNEL_PHYS_END;
#define KERNEL_PHYS_END ((uintptr_t)&__KERNEL_PHYS_END) 
#define KERNEL_VIRT_BASE 0xC0000000
/* its a hardcode address used in real mode already */
#define E820_TOTAL_ENTRIES 0x8000
#define E820_ENTRIES_BUFFER 0x8004

struct e820_entry {
    uint64_t base;
    uint64_t len;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed));

/*
 * frame management 
 */

#define FRAME_SIZE 4096
#define MAX_FRAMES (1 << 20) /* up to ~4GB / 4KB */


struct frame_table {
	uint64_t start;
	uint32_t total;
};

struct frame_table* frame_table;
uint8_t frame_bitmap[MAX_FRAMES / 8];

/* paging management */
/* paging normally would be after heap but I think its better provide basic 
 * paging for heap so we do not get engaged into frame managements and etc.
 */
#define PAGING_TOTAL_PG_DIR	1024
uint32_t page_directory[PAGING_TOTAL_PG_DIR];

#define PAGING_PAGE_PRESENT	0x1
#define PAGING_PAGE_RW		0x2
#define PAGING_PAGE_USER	0x4

/* heap management */
/* at start for our microkernel 16 MB or 4096 frame page (4KB each frame) */

#define HEAP_FRAME_RESERVE 	4096
#define HEAP_PAGE_SIZE		128

#define HEAP_TOTAL_ENTRIES	(FRAME_SIZE * HEAP_FRAME_RESERVE)/HEAP_PAGE_SIZE

/* we consider using specific VA for start of kernel heap */
#define HEAP_START_PDE_IDX	((KERNEL_VIRT_BASE >> 22) & 0x3ff)
#define HEAP_MAX_INIT_FRAME	4

#define HEAP_BLOCK_TAKEN	1
#define HEAP_BLOCK_FREE		0

/* each entry is 128b */
static uint8_t heap_bitmap[HEAP_TOTAL_ENTRIES];
static uint32_t heap_start = KERNEL_VIRT_BASE;

struct heap_header 
{
	/* number of heap blocks */
	size_t blocks;
};
/* ===================== Functions ========================*/
/* get free memory regions */
struct e820_entry* e820_get_memmap()
{
	uint32_t e820_total = *(volatile uint32_t*) E820_TOTAL_ENTRIES;
	struct e820_entry *entries = (struct e820_entry*) E820_ENTRIES_BUFFER;

	for (int i = 0; i < e820_total; i++)
	{
		if (entries[i].type == 1) 
		{
			kinfo("entries base is %lx", entries[i].base);
			if (entries[i].base <= KERNEL_PHYS_END && 
					entries[i].base + entries[i].len > KERNEL_PHYS_END)
			{
				return &entries[i];
			}
			if (entries[i].base > KERNEL_PHYS_END)
			{
				return &entries[i];
			}
		}

	}
	return NULL;
}

/* frame management */
static inline void set_frame(uint32_t frame)
{
	frame_bitmap[frame / 8] |= (1 << (frame % 8));
}

static inline void clear_frame(uint32_t frame)
{
	frame_bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static inline int test_frame(uint32_t frame)
{
	return frame_bitmap[frame / 8] & (1 << (frame % 8));
}

uint32_t frame_alloc()
{
	/* frame_table->total set in frame_init */
	for (uint32_t i = 0; i < frame_table->total; i++)
	{
		if (!test_frame(i))
		{
			set_frame(i);
			return i;
		}
	}
	return (uint32_t)-1;
}

void free_frame(uint32_t frame) 
{
	clear_frame(frame);
}

/* frame initialize */
void frame_init(struct e820_entry* entry)
{			
	uint64_t phys_end;
	uint32_t total_frames;

	memset(frame_table, 0, sizeof(struct frame_table));

	if (!entry)
		kpanic("entry for frame init is NULL");

	phys_end = (entry->base < KERNEL_PHYS_END) 
			? ((KERNEL_PHYS_END + 0xfff) & ~0xfff)
			: ((entry->base + 0xfff) & ~0xfff);			
	frame_table->start = entry->base;
	total_frames = ((phys_end - entry->base ) + FRAME_SIZE -1) / FRAME_SIZE;

	/* at this point alloc_frame know what is its max index frame */
	frame_table->total = total_frames;
}

/* paging */
static inline void* phys_to_virt(uint32_t phys) 
{
	return (void*)(phys + KERNEL_VIRT_BASE);
}

static inline void* virt_to_phys(uint32_t virt)
{
	return (void*)(uint32_t)virt - KERNEL_VIRT_BASE;
}

/* heap initialize */
void heap_init() 
{
	/* we assign 4 frame for addressing 16 MB */
	for (int i = 0; i < HEAP_MAX_INIT_FRAME; i++)
	{
		uint32_t pt_frame = frame_alloc();
		if (!pt_frame) 
			kpanic("cannot allocate frame for heap init");
		memset((void*)phys_to_virt(pt_frame), 0, FRAME_SIZE);
		
		page_directory[HEAP_START_PDE_IDX+i] = pt_frame | PAGING_PAGE_PRESENT |PAGING_PAGE_RW;
	}
}

void* kmalloc(size_t size)
{
	size_t blocks_needed = (size + HEAP_PAGE_SIZE - 1)/HEAP_PAGE_SIZE;

	/* we include a small header */
	size_t total_blocks = blocks_needed + 1;

	size_t free_count = 0;
	size_t start_idx = 0;

	for (size_t i = 0; i < HEAP_TOTAL_ENTRIES; i++)
	{
		if (heap_bitmap[i] == HEAP_BLOCK_FREE)
		{
			if (free_count == 0)
				start_idx = i;

			free_count++;

			if (free_count == total_blocks)
			{
				for (size_t j = start_idx; j < start_idx + blocks_needed; j++)
					heap_bitmap[j] = HEAP_BLOCK_TAKEN;

				struct heap_header* hdr =
					(struct heap_header*)(heap_start + start_idx * HEAP_PAGE_SIZE);
				hdr->blocks = total_blocks;

				return (void*)((uintptr_t)hdr + sizeof(struct heap_header));
			}
		} else {
			free_count = 0;
		}
	}

	/* no space found */
	return NULL;
}

void kfree(void* ptr)
{
	if (!ptr) return;

	struct heap_header* hdr = (struct heap_header*)((uintptr_t)ptr - sizeof(struct heap_header));
	size_t blocks = hdr->blocks;

	size_t start_idx = ((uintptr_t)hdr - heap_start) / HEAP_PAGE_SIZE;

	for (size_t j = start_idx; j < start_idx + blocks; j++)
		heap_bitmap[j] = HEAP_BLOCK_FREE;
}

/* mem, heap, paging initialization */
void mem_init()
{
	struct e820_entry* entry;
	entry = e820_get_memmap();
	if (!entry)
		kpanic("no usable memory region find by e820");
	
	frame_init(entry);

	/* frame is ready to be used in heap now */
	heap_init();
}
