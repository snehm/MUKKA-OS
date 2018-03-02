#include <hal.h>
#include <stdint.h>
#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

#include "../Kernel/kprint.h"

//hardware abstraction, implementaion of interfaces visible to portable code of our kernel

//hal initailization
int hal_initialize() {

   cpu_initialize();
   i86_pic_initialize (0x20,0x28);
   i86_pit_initialize (0x20);                   //install new interrupt vector for irq 32
   i86_pit_start_counter(1,I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);

   return 0;
}

//hal shutdown
int hal_shutdown() {
   
   cpu_shutdown();
   return 0;
}

//generate software interrupt
void generate_interrupt(int n) {

   asm( "movb %0, generateInt + 1\t\n"
        "jmp generateInt\n"
        "generateInt:\n\t"
        "int $0" 
        : /*no output "=r" */ : "r" (n));       
}

//read a byte from device port 
unsigned char inportb(unsigned short id) {

   unsigned char c;
   asm(  "movw %1, %%dx\t\n"
         "in %%dx, %%al\t\n"
         "movb %%al, %0\t\n"
         :"=r" (c) :"r" (id) :"%eax", "%edx"  );

   return (unsigned char)id;
}

//write a byte to device port
void outportb(unsigned short id, unsigned char value) {
   
   asm(  "movb %1, %%al \t\n"
         "movw %0, %%dx \t\n"
         "out %%al, %%dx \t\n"
         : :"r" (id), "r" (value) :"%eax", "%edx");
}

//enable hardware interrupt
inline void enable(){

  asm("sti\t\n");
}

//disable hardware interrupt
inline void disable(){

  asm("cli\t\n");
}

//interrupt done -> send EOI command to PIC
inline void interruptdone (uint32_t intno){

  if(intno > 15)
    return;

  //if interrupt line belongs to slave, we need to send EOI to both master and slave PIC
  if (intno >= 8)
    i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 1);

  //send EOI to master
  i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 0);
}

//return from intrrupt handler
inline void intret(){

  asm("iret\t\n");
}

//set new interrupt vector 
void setvect (int intno, void (*vect)(void)) {

  //install new ir
  idt_install_ir (intno, IDT_DESC_PRESENT | IDT_DESC_BIT32,
    0x8, vect);
}

//get interrupt routine
void* getvect (int intno) {

  //get interrupt vector
  struct idt_descriptor* desc = idt_get(intno);
  if (!desc)
    return 0;

  //address of handler
  uint32_t addr = desc->baseLow | (desc->baseHigh << 16);

  //return handler address
  void* irq = (void*)addr;

  return irq;
}

 int get_tick_count() {

  //return clock ticks
  return i86_pit_get_tick_count();
 }