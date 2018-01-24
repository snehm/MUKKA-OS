//must be compiled using gcc -masm=intel

#include <hal.h>
#include "cpu.h"
#include "idt.h"

#include "../Kernel/kprint.h"

//hardware abstraction, implementaion of interfaces visible to portable code of our kernel

//hal initailization
int hal_initialize() {

   cpu_initialize();
   return 0;
}

//hal shutdown
int hal_shutdown() {
   
   cpu_shutdown();
   return 0;
}

//generate software interrupt
void generate_interrupt(int n) {
   asm( "mov byte ptr [generateInt+1], %0\n\t"
        "jmp generateInt\n"
        "generateInt:\n\t"
        "int 0" 
        : /*no output "=r" */ : "r" (n));
        
}



