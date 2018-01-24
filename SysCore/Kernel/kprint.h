#ifndef _KPRINT_H
#define _KPRINT_H

#define BLACK 0
#define BLUE  1
#define GREEN 2
#define RED   4
#define WHITE 15

extern void kclearScreen(unsigned short c);
extern void kprintString(char *str);
extern int kprintf(const char* str, ...);
extern void ksetColor(const unsigned c);
extern void kgotoXY(unsigned x, unsigned y);

#endif
