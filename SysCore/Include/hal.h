#ifndef _HAL_H
#define _HAL_H

//initiaize Hardware abstraction layer
extern int hal_initialize();

//shutdown Hardware abstraction layer
extern int hal_shutdown();

//generate interruppt
extern void generate_interrupt(int i);

//read byte from device port 
extern unsigned char inportb(unsigned short id);

//write byte to device port
extern void outportb(unsigned short id, unsigned char value);

//enable hardware interrupt
extern void enable();

//disable hardware interrupt
extern void disable();

//send EOI command to PIC
extern void interruptdone (unsigned int intno);

//return from interupt handler
extern void intret();

//set new interrupt vector for an interrupt
extern void setvect (int intno, void (*vect)(void));

//get interrupt routine
extern void* getvect (int intno);

//get current clock tick count
extern int get_tick_count();

#endif
