#ifndef VGA_H
#define VGA_H

#define VGA_TEXT_MEM 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define LOG_INFO 0 
#define	LOG_ERR 1

#define VGA_TAB_SIZE 4

extern const char* log_level_prefix[];
extern const int log_level_colour[];

void vga_init();
void kputc(int level, char c);
void kputs(int level, const char* str);

void scroll_up();

#endif