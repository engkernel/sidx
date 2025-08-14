#include "string.h"

int strlen(const char* str)
{
	int i = 0;
	while (*str != 0)
	{
		i++;
		str++;
	}

	return i;
}

// TODO I think it does not works properly check again
char* itoa(int val, char* buf, int base)
{
	const char digits[] = "0123456789ABCDEF";
	char tmp_char;
	char* ptr = buf;
	char* ptr1 = buf;
	int tmp_val;

	if (base < 2 || base > 16)
	{
		*buf = '\0';
		return buf;
	}

	if (val < 0 && base == 10)
	{
		*ptr++ = '-';	
		val = -val;
	}

	do
	{
		tmp_val = val;
		val /= base;
		*ptr++ = digits[tmp_val % base];		
	}
	while (val);

	*ptr-- = '\0';

	if (*buf == '-')
	{
		ptr1++;
	}

	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

	return buf;
}