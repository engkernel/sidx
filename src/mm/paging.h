#ifndef PAGING_H
#define PAGING_H

#define PAGING_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096
#define PAGING_IS_WRITEABLE 0b00000010 

struct vmm_context
{
	uint32_t* page_directory;
};

#endif
