#include "error.h"
#include "log.h"

#define kpanic(fmt, ...)\
	do {\
		vprintf(LOG_ERR, "[PANIC][%s %s: %d" fmt " \n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);\
		while(1){};\
	} while(0)
