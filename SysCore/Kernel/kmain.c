//build :: cc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -c -o kernel.o kmain.c

#include "kprint.h"
#include <string.h>
#include <hal.h>

void kmain(){
    kclearScreen(1);
    
    char *s = "Welcome to MUKKA OS\n\0";
    ksetColor(7);
    kgotoXY(25, 2); 
    kprintString(s); 
    
    kgotoXY(0, 3);
    kprintString("Initializing HAL ...\n\0");
    hal_initialize();
   
    //generate an interrupt
    generate_interrupt(0);
      
}
