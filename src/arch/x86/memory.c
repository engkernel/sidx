#include "arch.h"
#include "include/log.h"
#include "include/string.h"

#include <stdbool.h>
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
#define PAGING_PAGE_SIZE	4096
#define PAGING_TOTAL_ENTRIES	1024

#define PAGING_PAGE_PRESENT	0x1
#define PAGING_PAGE_RW		0x2
#define PAGING_PAGE_USER	0x4

__attribute__((aligned(PAGING_PAGE_SIZE))) uint32_t page_directory[PAGING_TOTAL_ENTRIES];
__attribute__((aligned(PAGING_PAGE_SIZE))) uint32_t first_page_table[PAGING_TOTAL_ENTRIES];

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
#define HEAP_START KERNEL_VIRT_BASE

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
	return (frame_bitmap[frame / 8] & (1 << (frame % 8))) != 0;
}

uint32_t frame_alloc()
{
	/* frame_table->total set in frame_init */
	for (int i = 0; i < frame_table->total; i++)
	{
		if (!test_frame(i))
		{
			set_frame(i);
			return frame_table->start + (i * FRAME_SIZE);
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
	uint64_t phys_start, phys_end;
	uint32_t total_frames;

	memset(frame_table, 0, sizeof(struct frame_table));

	if (!entry)
		kpanic("entry for frame init is NULL");

	phys_start = (entry->base < KERNEL_PHYS_END)
		? ((KERNEL_PHYS_END + FRAME_SIZE -1) & ~(FRAME_SIZE -1))
		: ((entry->base + FRAME_SIZE - 1) & ~(FRAME_SIZE -1));

	phys_end = (entry->base + entry->len) & ~(FRAME_SIZE - 1);

	if (phys_end <= phys_start)
		kpanic("invalid frame range");
	
	frame_table->start = phys_start;;
	total_frames = (phys_end - phys_start) / FRAME_SIZE;

	kinfo("total frames that we gathered %d", total_frames);
	/* at this point alloc_frame know what is its max index frame */
	frame_table->total = total_frames;
	for (int i = 0; i < (MAX_FRAMES/8); i++)
		clear_frame(i);
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

void paging_init()
{
	uint32_t cr0;

	for (int i = 0; i < PAGING_TOTAL_ENTRIES; i++)
	{
		page_directory[i] = 0x00000002; // not present, RW
	}

	for (uint32_t addr = 0; addr < 0x00400000; addr += PAGING_PAGE_SIZE)
	{
		first_page_table[addr/PAGING_PAGE_SIZE] = addr | PAGING_PAGE_PRESENT | PAGING_PAGE_RW;
	}

	uint32_t pt_phys = (uint32_t)first_page_table;
	page_directory[0] = pt_phys | PAGING_PAGE_PRESENT | PAGING_PAGE_RW;

	/* consider heap PDE now */
	page_directory[HEAP_START_PDE_IDX] = 0x00000002; // not present, RW

	/* load CR3 with page directory */
	__asm__ volatile("mov %0, %%cr3" :: "r"(page_directory));

	/* enable paging */
	__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	__asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
}

void map_page(uint32_t virt, uint32_t phys, uint32_t flags)
{
	uint32_t dir_idx; 
	uint32_t tbl_idx;
	uint32_t* page_table;
	uint32_t pt_phys;

	// 1. directory index and table index
	dir_idx	= (virt >> 22) & 0x3ff;
	tbl_idx	= (virt >> 12) & 0x3ff;

	// 2. get PDE
	if (page_directory[dir_idx] & PAGING_PAGE_PRESENT)
	{
		page_table = (uint32_t*)(page_directory[dir_idx] & ~0xfff);
	} 
	else
	{
		// allocate new page table
		pt_phys = frame_alloc();
		page_table = (uint32_t*)phys_to_virt(pt_phys);
		memset(page_table, 0, PAGING_PAGE_SIZE);

		// add to page directory
		page_directory[dir_idx] = pt_phys | PAGING_PAGE_PRESENT | PAGING_PAGE_RW;
	}

	// 3. add mapping to page table
	page_table[tbl_idx] = (phys & ~0xfff) | (flags & 0xfff);

	// 4. invalidate TBL for this page
	__asm__ volatile("invlpg (%0)" :: "r"(virt) : "memory");
}

bool is_mapped(uint32_t virt_addr)
{
	uint32_t dir_idx = (virt_addr >> 22) & 0x3ff;
	uint32_t tbl_idx = (virt_addr >> 12) & 0x3ff;
	
	uint32_t pde = page_directory[dir_idx];
	if (!(pde & 0x1))
		return false;

	uint32_t* page_table = (uint32_t*)phys_to_virt(pde & 0xfffff000);

	uint32_t pte = page_table[tbl_idx];
	return (pte & 0x1) != 0; // present bit set?
}

#define HEAP_TEMP_VIRT 0x003f0000
/* heap initialize */
void heap_init() 
{
	uint32_t pt_phys = frame_alloc();
	uint32_t* pt_virt = (uint32_t*)HEAP_TEMP_VIRT;

	memset(pt_virt, 0, PAGING_PAGE_SIZE);

	page_directory[HEAP_START_PDE_IDX] = pt_phys | PAGING_PAGE_PRESENT | PAGING_PAGE_RW;

	uint32_t phys = frame_alloc();
	pt_virt[0] = phys | PAGING_PAGE_PRESENT | PAGING_PAGE_RW;

}

void* grow_heap(uint32_t virt_addr)
{
	uint32_t phys = frame_alloc();
	map_page(virt_addr, phys, PAGING_PAGE_PRESENT | PAGING_PAGE_RW);
	return (void*)virt_addr;
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
				for (size_t j = start_idx; j < start_idx + total_blocks; j++)
					heap_bitmap[j] = HEAP_BLOCK_TAKEN;

				uintptr_t virt_addr = HEAP_START+(start_idx * HEAP_PAGE_SIZE);
				for (size_t j = 0; j < total_blocks; j++)
				{
					uintptr_t v = virt_addr + j * HEAP_PAGE_SIZE;

					if (!is_mapped(v))
					{
						uint32_t phys = frame_alloc();
						if (phys == (uint32_t)-1)
							return NULL;
						map_page(v, phys, PAGING_PAGE_PRESENT | PAGING_PAGE_RW);
					}
				}

				struct heap_header* hdr =
					(struct heap_header*)virt_addr;
				hdr->blocks = total_blocks;
				kinfo("hdr addr %x", hdr);
				kinfo("kmalloc hdr->blocks %d and total blocks %d", hdr->blocks, total_blocks);

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
	size_t start_idx = ((uintptr_t)hdr - HEAP_START) / HEAP_PAGE_SIZE;

	for (size_t j = 0; j < blocks; j++)
		heap_bitmap[start_idx+j] = HEAP_BLOCK_FREE;
}

/* mem, heap, paging initialization */
void mem_init()
{
	struct e820_entry* entry;
	entry = e820_get_memmap();
	if (!entry)
		kpanic("no usable memory region find by e820");
	
	frame_init(entry);

	/* paging */
	paging_init();

	/* frame is ready to be used in heap now */
	heap_init();

}
