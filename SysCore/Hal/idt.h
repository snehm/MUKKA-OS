#ifndef _IDT_H
#define _IDT_H

#include<stdint.h>

//maximum number of interrupts
#define MAX_INTERRUPTS 256

// Interrupt Descriptor table
struct idt_descriptor {
   //0-15 bits for lower bit address of interrupt routine
   uint16_t baseLow;
   
   //segment selector bit 16-31
   uint16_t selector;
   
   //reserved bit 32-39
   uint8_t reserved;
   
   //control flags 40-47
   uint8_t flags;
   
   //higher 16 bit address of interrrupt routine
   uint16_t baseHigh;
   
} __attribute__((packed)) ;

//IDT flag control

//16 bit descriptor
#define IDT_DESC_BIT16		0x06	   //00000110

//32 bit descriptor
#define IDT_DESC_BIT32		0x0E	   //00001110

//ring 1 privedlge
#define IDT_DESC_RING1		0x40	   //01000000

//ring 2 privledge
#define IDT_DESC_RING2		0x20	   //00100000

//ring 3 privledge
#define IDT_DESC_RING3		0x60	   //01100000

//if this descriptor in not used, set this bit to zero or set it to one
#define IDT_DESC_PRESENT   0x80	   //10000000


/*interface declaration*/

//returns interrupt descriptor for an interrupt
extern struct idt_descriptor* idt_get(uint32_t i);

//install idt for an interrupt i
extern int idt_install_ir(uint32_t i, uint8_t flags, uint16_t selector, void (*irq_handler)(void));

//initialize IDT
extern int idt_initialize(uint16_t codeSelector);

#endif
