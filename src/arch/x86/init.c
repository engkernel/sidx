#include "arch.h"

/* @arch_init */
void arch_init()
{
	/* disable interrupts for initialization arch */
	disable_interrupts();

	/* vga for debuging is needed */	
	vga_init();

	/* gdt initalize */
	gdt_init();
	
	/* interrupts and lidt initialize */
	traps_init();

	/* enable interrupts as arch initialized */
	enable_interrupts();
}

