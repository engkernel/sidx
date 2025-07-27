#ifndef BUDDY_H
#define BUDDY_H

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096
#define MAX_ORDER 10

#define BUDDY_MEM_BASE 0x01000000

void buddy_init(uintptr_t base, size_t size);
void* buddy_alloc(uint8_t order);
void buddy_free(void* ptr, uint8_t order);
int size_to_order(size_t size);
int addr_to_order(void* ptr);

#endif
