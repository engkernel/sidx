#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGING_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096
#define PAGING_IS_WRITEABLE 0b00000010 

struct vmm_context
{
	uint32_t* page_directory;
};

struct vmm_context* new_vmm_context(uint8_t flags);
void vm_change_directory(struct vmm_context* vm);
void load_page_directory(uint32_t* directory);
void enable_paging();

#endif
