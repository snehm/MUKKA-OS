#include "pit.h"
#include "idt.h"
#include <hal.h>
#include <stdint.h>

//to print messege
#include "../Kernel/kprint.h"

#define I86_PIT_REG_COUNTER0     0x40
#define I86_PIT_REG_COUNTER1     0x41
#define I86_PIT_REG_COUNTER2     0x42
#define I86_PIT_REG_COMMAND      0x43

//global tick count
uint32_t _pit_ticks = 0;

//pit interrupt handler, will be called by wraper function in assembly pit_handler_wrapper.s
void i86_pit_irq() {

   ksetColor(GREEN);
   kprintString("\n[x86]: clock tick\n\0");
   _pit_ticks++;


   //send EOI to PIC
   interruptdone(0);

}

//send command to pit
void i86_pit_send_command(uint8_t cmd) {

   outportb(I86_PIT_REG_COMMAND, cmd);
}

//send data to counter
void i86_pit_send_data (uint16_t data, uint8_t counter ) {

   //port of the counter
   uint8_t port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 : 
            ((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

   outportb(port, data);
}

//read data from counter
uint8_t i86_pit_read_data (uint16_t counter) {

   //port of the counter
   uint8_t port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 : 
            ((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

   return inportb(port); 
}

//initalize pit
void i86_pit_initialize (uint8_t irq) {

   //install new interrupt vector
   setvect(irq, i86_pic_handler_wrapper);
}

//start counter
void i86_pit_start_counter(uint32_t freq, uint8_t counter, uint8_t mode) {

   if (freq == 0)
      return;

   //count = pit frequency / freq
   uint16_t divisor = 1193182 / freq; 

   //send operational comand
   uint8_t ocw = 0;
   ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode;
   ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
   ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | counter;
   i86_pit_send_command(ocw);

   //set clock frequency rate

   //send LSB first
   i86_pit_send_data(divisor & 0xff, 0);
   i86_pit_send_data((divisor >> 8) & 0xff, 0);

   //reset tick count
   _pit_ticks = 0;
}

//get clock ticks
uint32_t i86_pit_get_tick_count (){

   return _pit_ticks;
}

//set clock ticks
uint32_t i86_pit_set_tick_count (uint32_t x) {

   _pit_ticks = x;
   return x;
}