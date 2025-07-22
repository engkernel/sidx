#include "mm.h"
#include "utils/log.h"

void* memset(void* dest, int val, unsigned int size)
{
	char* ptr = (char*)dest;
	while(size--)
	{
		*ptr++ = (char)val;
	}

	return dest;
}

void* memcpy(void* dest, void* src, int len)
{
	char* d = dest;
	char* s = src;
	while(len--)
	{
		*d++ = *s++;
	}
	return dest;
}

