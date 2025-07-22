#ifndef KHEAP_H
#define KHEAP_H

void* kmalloc(size_t size);
void kfree(void* addr);
void* kzalloc(size_t size);

#endif
