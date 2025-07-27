#include "tss.h"
#include "mm/mm.h"
#include "mm/buddy.h"
#include "utils/log.h"


void tss_init(struct tss* tss) 
{
    memset(tss, 0x00, sizeof(struct tss));
    tss->esp0 = (uint32_t) buddy_alloc(0);
    tss->ss0 = KERNEL_DATA_SELECTOR;
    tss_load(0x28);
}

