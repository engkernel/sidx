#ifndef LOG_H
#define LOG_H

#include "vga.h"

void vprintf(int level, const char* fmt, ...);

#define kinfo(fmt, ...)\
    do {\
        vprintf(LOG_INFO, "[INFO][%s %s:%d] " fmt " \n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);\
    } while(0)
#define kerr(fmt, ...)\
    do {\
        vprintf(LOG_ERR, "[ERROR][%s %s:%d] " fmt " \n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);\
    } while(0)

#endif
