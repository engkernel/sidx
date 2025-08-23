#include "include/string.h"
#include <stddef.h>

void *memset(void *s, int c, size_t len)
{
	char *tmp = s;
	while (len--)
		*tmp++ = c;

	return s;
}

void *memcpy(void *dst, void *src, size_t len)
{
	char *tmp = dst;
	const char *s = src;

	while(len--)
		*tmp++ = *s++;

	return dst;
}

size_t strlen(char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc);

	return sc - s;
}
