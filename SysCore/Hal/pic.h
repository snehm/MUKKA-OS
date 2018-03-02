#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>

// PIC I interrupt lines
#define I86_PIC_IRQ_TIMER        0
#define I86_PIC_IRQ_KEYBOARD     1
#define I86_PIC_IRQ_SERIAL2      3
#define I86_PIC_IRQ_SERIAL1      4
#define I86_PIC_IRQ_PARALLEL2    5
#define I86_PIC_IRQ_DISKETTE     6
#define I86_PIC_IRQ_PARALLEL1    7

// PIC II inturrupt lines
#define I86_PIC_IRQ_CMOSTIMER    0
#define I86_PIC_IRQ_CGARETRACE   1
#define I86_PIC_IRQ_AUXILIARY    4
#define I86_PIC_IRQ_FPU          5
#define I86_PIC_IRQ_HDC          6

//operational command I donot have any spaecial format, its read from IMR

//operational command II, used to control PIC
#define I86_PIC_OCW2_MASK_L1     1              //00000001 -> interrupt level 1
#define I86_PIC_OCW2_MASK_L2     2              //00000010 -> interrupt level 2
#define I86_PIC_OCW2_MASK_L3     4              //00000100 -> interrupt level 3
#define I86_PIC_OCW2_MASK_EOI    0x20           //00010000 -> end of interrupt
#define I86_PIC_OCW2_MASK_SL     0x40           //01000000 -> select command
#define I86_PIC_OCW2_MASK_ROTATE 0x80           //10000000 -> rotation command

//operational command III, NOT required for now so we will add later


//read data from pic
extern uint8_t i86_pic_read_data (uint8_t picNum);

//send data to pic
extern void i86_pic_send_data (uint8_t data, uint8_t picNum);

//send operational command to pic
extern void i86_pic_send_command (uint8_t cmd, uint8_t picNum);

//enables and disables interrupts
extern void i86_pic_mask_irq (uint8_t irqmask, uint8_t picNum);

//Initialize pic
extern void i86_pic_initialize (uint8_t base0, uint8_t base1);

#endif