#include "cpu.h"
#include "gdt.h"
#include "idt.h"


//cpu initailization
int cpu_initialize() {
   gdt_initialize();
   
   //0x8 gdt default code descriptor
   idt_initialize(0x8);
   return 0;
}


//cpu shutdown
void cpu_shutdown() {
   //yet to implement
}

