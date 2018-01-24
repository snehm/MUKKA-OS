/* build with gcc -masm=intel
*/

#include "idt.h"
#include <_null.h>
#include <stdint.h>
#include <string.h>

//to print messege
#include "../Kernel/kprint.h"

//idtr stucture, to install in idtr register
struct idtr {
   
   //size of idt table
   uint16_t size;
   
   //base address of idt
   uint32_t base;
} __attribute__((packed));


//iterrupt descriptor table
struct idt_descriptor _idt [ MAX_INTERRUPTS];

//idtr registern content
struct idtr _idtr;

//installs out idt in idtr
static void idt_install(){
   asm("lidt [%0]" :: "r" (&_idtr));
} 

//default handler for our interrupts
static void default_handler() {
   ksetColor(RED);
   kprintString("\n[x86]: unhandled exception caught\n\0");
   
   for(;;);       // we need to fix this!!
}

//get interrupt descriptor for interrupt i
struct idt_descriptor* get_idt(uint32_t i) {
   if(i >= MAX_INTERRUPTS )
      return NULL;
   
   return &_idt[i];
}

//install a new interrupt descriptor for an interrupt i
int idt_install_ir(uint32_t i, uint8_t flags, uint16_t selector, void (*irq_handler)(void)) {
   
   //if invalid
   if(i >= MAX_INTERRUPTS)
      return 0;
   
   //if null
   if(!irq_handler) 
      return 0;
   
   _idt[i].baseLow		=	(uint16_t) ( (uint32_t)irq_handler & 0xffff );
	_idt[i].baseHigh		=	(uint16_t) ( ((uint32_t)irq_handler >> 16) & 0xffff);
	_idt[i].reserved	   =	0;
	_idt[i].flags		   =	(uint8_t) (flags);
	_idt[i].selector	   =	selector;

	return	0;
}

//initalize interrupt descriptor table
int idt_initialize (uint16_t codeSelector) {

	//set idtr
	_idtr.size  = sizeof(struct idt_descriptor) * MAX_INTERRUPTS -1;
	_idtr.base	= (uint32_t)&_idt[0];

	//zero out idt 
	memset ((void*)&_idt[0], 0, sizeof (struct idt_descriptor) * MAX_INTERRUPTS-1);

	//register default handler to all interrupt
	for (int i=0; i<MAX_INTERRUPTS; i++)
		idt_install_ir (i, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector , default_handler);

	//install our idt
	idt_install ();

	return 0;
}
