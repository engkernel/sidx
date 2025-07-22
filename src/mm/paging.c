#include "paging.h"
#include "heap.h"
#include <stdint.h>

extern void load_page_directory(uint32_t* directory);
extern void enable_paging();

uint32_t* current_directory = 0;
struct vmm_context* new_vmm_context(uint8_t flags)
{
	uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_ENTRIES_PER_TABLE);	
	int offset = 0;
	for (int i = 0; i < PAGING_ENTRIES_PER_TABLE; i++)
	{
		uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_ENTRIES_PER_TABLE);
		for (int j = 0; j < PAGING_ENTRIES_PER_TABLE; j++)
		{
			entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
		}
		offset += PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE; 
		directory[i] = (uint32)entry | flags | PAGING_IS_WRITEABLE;
	}
	struct vmm_context* vm = kzalloc(sizeof(struct vmm_context));
	vm->page_directory = directory;
	return vm;
}

void vm_change_directory(struct vmm_context* vm)
{
	load_page_directory(vm->page_directory);
	current_directory = vm->page_directory;
}

void vm_get_indexes(void* virtual_address, uint32_t* dir_index, uint32_t* table_index)
{
	*dir_index = ((uint32_t)virtual_address / (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
	*table_index = ((uint32_t) virtual_address % (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) /  PAGING_PAGE_SIZE);
}

uint32_t vm_get_page(uint32_t* directory, void* virt)
{
	uint32_t dir_index = 0;
	uint32_t table_index = 0;
	vm_get_indexes(virt, &dir_index, &table_index);
	uint32_t entry = directory[dir_index];
	uint32_t* table = (uint32_t*)(entry & 0xfffff000);
	return table[table_index];
}

int vm_set_page(uint32_t* directory, void* virt, val)
{
	uint32_t dir_index = 0;
	uint32_t table_index = 0;
	int res = vm_get_indexes(virt, &dir_index, &table_index);
	if (res < 0)
	{
		return res;
	}

	uint32_t entry = directory[dir_index];
	uint32_t* table = (uint32_t*)(entry & 0xfffff000);
	table[table_index] = val;

	return 0;
}

void free_vm_context(struct vm_context* vm)
{
	for (int i = 0; i < PAGING_ENTRIES_PER_TABLE; i++)
	{
		uint32_t entry = vm->page_directory[i];
		uint32_t* table = (uint32_t*)(entry & 0xfffff000);
		kfree(table);
	}
	kfree(vm->page_directory);
	kfree(vm);
}

void vmm_init()
{

}
