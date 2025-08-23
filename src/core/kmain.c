#include "include/kmain.h"
#include "include/arch.h"
#include "include/log.h"

void kmain()
{
	arch_init();	
	kinfo("hello world");
	while(1){}
}
