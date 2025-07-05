#include "vga.h"
#include "mm/mm.h"
#include "string.h"
#include <stdint.h>
#include <stddef.h>

uint16_t* vga_mem = 0;
uint16_t vga_row = 0;
uint16_t vga_col = 0;

const char* log_level_prefix[] = 
{
	[LOG_INFO] = "[INFO]",
	[LOG_ERR] = "[ERROR]"
};

const int log_level_colour[] =
{
	[LOG_INFO] = 0x0F,
	[LOG_ERR] = 0x0C
};

static inline void __kmake_c(int x, int y, char c, char colour) 
{
	vga_mem[(y * VGA_WIDTH) + x] = ((colour << 8) | c);
}

static inline void __kputc(int level, char c)
{
	int colour = log_level_colour[level];
	if (level < LOG_INFO || level > LOG_ERR)
	{
		level = LOG_ERR;
	}

	if (c == '\n')
	{
		vga_col = 0;
		vga_row++;
		return;
	} 
	else if (c == '\t')
	{
		if (vga_col + VGA_TAB_SIZE >= VGA_WIDTH)
		{
			vga_col = 0;
			vga_row++;
		} 
		else 
		{
			vga_col += VGA_TAB_SIZE;
		}
		return;
	}
	__kmake_c(vga_col, vga_row, c, colour);
	vga_col += 1;
	if (vga_col >= VGA_WIDTH)
	{
		vga_col = 0;
		vga_row += 1;
	}
}

void kputc(int level, char c)
{
	__kputc(level, c);
}

static void __kputs(int level, const char* str)
{
	if (!str)
		return;
	size_t len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		__kputc(level, str[i]);
	}
}

void kputs(int level, const char* str)
{
	__kputs(level, str);
}

static inline void __k_cls()
{
	for (int y = 0; y < VGA_HEIGHT; y++)
	{
		for (int x = 0; x < VGA_WIDTH; x++)
		{
			__kmake_c(x, y, ' ', 0);
		}
	}
}

void cls()
{
	__k_cls();	
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
	cls();
}

