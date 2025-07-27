#include "kmain.h"
#include "gdt.h"
#include "utils/vga.h"
#include "utils/log.h"
#include "arch/x86/interrupt/idt.h"
#include "arch/x86/mmap/mmap.h"
#include "mm/pmm.h"
#include "task/task.h"
#include "task/tss.h"
#include "mm/paging.h"


// static struct vmm_context* kernel_vm;
// GDT Entries
struct tss tss;
struct segment_descriptor segment_desc[TOTAL_GDT_ENTRIES];
struct gdt gdt[TOTAL_GDT_ENTRIES];

void kmain() 
{
	vga_init();
	kinfo("Initialize kmain");
	pmm_init();
	gdt_init(gdt, segment_desc, &tss);
	idt_init();
	
	// print_all_e820_regions();
	tss_init(&tss);
	// enable_paging();
	task_init();

	// kernel_vm = new_vmm_context(PAGING_IS_WRITEABLE);
	// vm_change_directory(kernel_vm);

	enable_interrupt();

	do_it();
	kpanic("end of kernel!");
}
