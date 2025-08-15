#include "kmain.h"
#include "debug/vga.h"
#include "debug/log.h"

void kmain()
{
	vga_init();	
	kinfo("hello world");
	while(1){}
}
