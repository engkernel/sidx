#include "log.h"
#include "vga.h"
#include "string.h"

#include <stdarg.h>
#include <stdint.h>

void static __print_udec32(int level, uint32_t num)
{
	char buf[11];

	int i = 0;
	
	if (num == 0)
	{
		kputc(level, '0');
		return;
	}

	while (num >0)
	{
		buf[i++] = '0' + (num % 10);
		num /= 10;
	}
	while(i--)
	{
		kputc(level, buf[i]);
	}
}

void static __print_udec64(int level, uint64_t num)
{
	// char buf[21];

	// int i = 0;
	
	// if (num == 0)
	// {
	// 	kputc(level, '0');
	// 	return;
	// }

	// while (num >0)
	// {
	// 	buf[i++] = '0' + (num % 10);
	// 	num /= 10;
	// }
	// while(i--)
	// {
	// 	kputc(level, buf[i]);
	// }
}

void static __print_dec32(int level, int32_t num)
{
	char buf[12];
	int i = 0;

	if (num == 0)
	{
		kputc(level, '0');
		return;
	}

	if (num < 0)
	{
		kputc(level, '-');
		num = -num;
	}

	while (num >0)
	{
		buf[i++] = '0' + (num % 10);
		num /= 10;
	}
	while(i--)
	{
		kputc(level, buf[i]);
	}
}

void static __print_dec64(int level, int32_t num)
{
	char buf[21];
	int i = 0;
	if (num == 0)
	{
		kputc(level, '0');
		return;
	}

	if (num < 0)
	{
		kputc(level, '-');
		num = -num;
	}

	while (num >0)
	{
		buf[i++] = '0' + (num % 10);
		num /= 10;
	}
	while(i--)
	{
		kputc(level, buf[i]);
	}
}

void static __print_hex32(int level, uint64_t num)
{
	char hex_chars[] = "0123456789abcdef";
	for (int i = 28; i >= 0; i -= 4)
		kputc(level, hex_chars[(num >> i) & 0xF]);
}

void static __print_hex64(int level, uint64_t num)
{
	char hex_chars[] = "0123456789abcdef";
	for (int i = 60; i >= 0; i -= 4)
		kputc(level, hex_chars[(num >> i) & 0xF]);
}

void static __print_ptr32(int level, void* ptr)
{
	kputs(level, "0x");
	__print_hex32(level,(uint32_t)(uintptr_t)ptr);
}

void static __print_ptr64(int level, void* ptr)
{
	kputs(level, "0x");
	__print_hex64(level,(uint64_t)(uintptr_t)ptr);
}

void static __printf(int level, const char *fmt, va_list args)
{
	while(*fmt)
	{
		if (*fmt != '%')
		{
			kputc(level, *fmt++);
			continue;
		}
		fmt++;

		int is_long = 0;

		if (*fmt == 'l')
		{
			is_long = 1;
			fmt++;
		}

		switch (*fmt)
		{
		case 'u':
		{
			is_long ?
				__print_udec64(level, va_arg(args, uint64_t)):
				__print_udec32(level, va_arg(args, uint32_t));
		}
		case 'd':
		{
			is_long ?
				__print_dec64(level, va_arg(args, int64_t)):
				__print_dec32(level, va_arg(args, int32_t));
			break;
		}
		case 'x':
		{
			is_long ? 
				__print_hex64(level, va_arg(args, uint64_t)) : 
				__print_hex32(level, va_arg(args, uint32_t));
			break;
		}	
		case 'p':
		{
			is_long ? 
				__print_ptr64(level, va_arg(args, void*)) : 
				__print_ptr32(level, va_arg(args, void*));
			break;
		}
		case 's':
		{
			kputs(level, va_arg(args, const char *));
		}
		default:
			break;
		}
		fmt++;
	}
}

void vprintf(int level, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	__printf(level, fmt, args);
	va_end(args);
}