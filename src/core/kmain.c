#include "include/kmain.h"
#include "include/arch.h"
#include "include/log.h"

void kmain()
{
	int *buf, *tmp;
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
	kinfo("buffer is %x %x %x %x", &buf[0], &buf[1], &buf[2], &buf[3]);

	tmp = kmalloc(54);
	kinfo("tmp %x", tmp);
	kfree(buf);

	buf = kmalloc(4);
	kinfo("buf %x", buf);
	while(1){}
}
