#include<string.h>
#include<size_t.h>

size_t strlen( const char* str ){
   size_t len = 0;
   while(str[len++]);
   return len;
}

void strcpy(char* dest, const char* src){
   
   int i;
   for(i=0; src[i] != '\0'; i++)
      dest [i] = src [i];
   
   dest [i] = '\0';   
}

void* memset(void* mem, unsigned char value, size_t count){
   for(int i=0; i<count; i++)
      ((unsigned char *)mem) [i] = value;
   
   return (void *)mem;
}

void* memcpy(void* desc, const void* src, size_t count){
   for(int i=0; i<count; i++)
      ((unsigned char*)desc) [i] = ((unsigned char* )src) [i] ;
   
   return (void *)desc;
}


