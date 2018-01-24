#include <stdarg.h>                          // for variable arguments
#include <string.h>
#include "kprint.h"

#define VID_MEMORY 0xB8000                  // VIDEO MEMORY ADDRESS            
#define COLS 80                             // MOD 7 display
#define LINES 25

static unsigned int _xPos = 0;
static unsigned int _yPos = 0;
static unsigned int _color = 30;

void ksetColor(unsigned c){
   _color = c;
}

void kclearScreen(unsigned short c){       // its easy
   unsigned char* p = (unsigned char*)VID_MEMORY;
   for(int i=0; i<COLS*LINES*2; i += 2){
      p[i] = ' ';
      p[i+1] = c;
   }
}

void kgotoXY(unsigned x, unsigned y){
   _xPos = x % COLS;
   _yPos = y % LINES;
}

void kprintChar(unsigned char c){
   if(c == 0)
      return;
   
   if(c == '\n' || c == '\r'){
      _yPos += 1;
      _yPos %= LINES;
      _xPos = 0;
      return;
   }
   
   if(_xPos > 79){
      _yPos += 1;
      _yPos %= LINES;
      _xPos = 0;
   }
   
   unsigned char* p = (unsigned char *)VID_MEMORY + (_yPos*80*2 + (_xPos++)*2);
   *p++ = c;
   *p = _color;
}

void kprintString(char *s){
   for(int i=0; i<strlen(s); i++)
      kprintChar(s[i]);
}
