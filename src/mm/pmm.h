#ifndef PMM_H
#define PMM_H

#include <stddef.h>

#define BUDDY_POOL_ADDRESS 0x01000000
#define BUDDY_POOL_SIZE    104857600
void pmm_init();

#endif
