#include "include/kmain.h"
#include "include/arch.h"
#include "include/log.h"

void kmain()
{
	int *buf;
	arch_init();	
	kinfo("hello world");
	buf = kmalloc(32);
	if (!buf)
		kpanic("buffer not allocated");
	buf[0] = 21;
	kinfo("buf 0 %d", buf[0]);
	buf[1] = 32;
	buf[2] = 23;
	buf[3] = 3;
	kinfo("buffer is %d %d %d %d", buf[0], buf[1], buf[2], buf[3]);
	while(1){}
}
