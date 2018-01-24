#ifdef __STDARG_H
#define __STDARG_H

#include<va_list.h>

#define STACKITEM int

//return size of an item pushed on stack
#define VA_SIZE(TYPE)            \
   (( sizeof(TYPE) + sizeof(STACKITEM) - 1)     \
       & ~(sizeof(STACKITEM) - 1))

//&LASTARG points to leftmost argument before ...
#define va_start(AP, LASTARG)       \
   (AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

//nothing for va_end
#define va_end(AP)

#define va_arg(AP, TYPE)    \
   (AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))
   
#endif
