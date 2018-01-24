#ifndef _STRING_H
#define _STRING_H

#include<size_t.h>

extern size_t strlen ( const char* str );
extern void strcpy (char* dest, const char* src);

extern void* memset(void* mem, unsigned char value, size_t count);
extern void* memcpy(void *dest, const void* src, size_t count); 

#endif
