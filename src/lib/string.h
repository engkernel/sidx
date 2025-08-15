#ifndef STRING_H
#define STRING_H

#include <stddef.h>

/* memory funcs */
void *memset(void *s, int c, size_t len);
void *memcpy(void *dst, void *src, size_t len);

/* string funcs */
size_t strlen(char *s);
#endif
