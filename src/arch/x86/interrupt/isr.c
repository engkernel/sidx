#include "isr.h"
#include "utils/log.h"
#include "arch/x86/io/io.h"

void empty_handler()
{
	outb(0x20, 0x20);
}

void keyboard_handler()
{
	kinfo("keyboard pressed!");
	outb(0x20, 0x20);
}
