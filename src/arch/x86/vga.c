#include "arch.h"
#include "include/string.h"
#include "include/configs.h"
#include <stdint.h>
#include <stddef.h>

uint16_t* vga_mem = 0;
uint16_t vga_row = 0;
uint16_t vga_col = 0;

const int log_level_colour[] =
{
	[LOG_INFO] = 0x0F,
	[LOG_ERR] = 0x0C
};

static void __vga_new_line()
{
	vga_col = 0;
	vga_row += 1;
}

void vga_new_line()
{
	__vga_new_line();
}

void vga_tab()
{
	if (vga_col + VGA_TAB_SIZE >= VGA_WIDTH)
	{
		__vga_new_line();
		return;
	}
	vga_row += VGA_TAB_SIZE;
}


static inline void __vga_c(int x, int y, char c, char colour) 
{
	vga_mem[(y * VGA_WIDTH) + x] = ((colour << 8) | c);
}

static inline void __vga_put(int level, char c)
{
	int colour = log_level_colour[level];

	__vga_c(vga_col, vga_row, c, colour);

	vga_col += 1;
	if (vga_col >= VGA_WIDTH)
	{
		__vga_new_line();
	}
}

void vga_put(int level, char c)
{
	__vga_put(level, c);
}

static inline void __vga_cls()
{
	for (int y = 0; y < VGA_HEIGHT; y++)
	{
		for (int x = 0; x < VGA_WIDTH; x++)
		{
			__vga_c(x, y, ' ', 0);
		}
	}
}

void cls()
{
	__vga_cls();	
}

void scroll_up()
{
	unsigned short *vga_buffer = (unsigned short*) VGA_TEXT_MEM;
	int i;
	for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++)
	{
		vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
	}

	for (i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
	{
		vga_buffer[i] = (0x07 << 8) | ' ';
	}
}

void vga_init()
{
	vga_mem = (uint16_t*)(VGA_TEXT_MEM);	
	__vga_cls();
}
