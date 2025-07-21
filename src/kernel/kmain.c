#include "kmain.h"
#include "utils/vga.h"
#include "utils/log.h"
#include "arch/x86/interrupt/idt.h"
#include "arch/x86/mmap/mmap.h"
#include "mm/pmm.h"
#include "task/task.h"

void kmain() 
{
	vga_init();
	idt_init();
	kinfo("Initialize kmain");
	enable_interrupt();
	// print_all_e820_regions();
	pmm_init();
	task_init();
	do_it();
	kinfo("end of kernel!");
}
