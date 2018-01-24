#ifndef _GDT_H
#define _GDT_H

/*
   * Global Descriptor Table for i86 processor
   * defines gdt interfaces that can be managed by HAL
*/

#include <stdint.h>

//NO of descriptor
#define GDTMAX 3

struct gdt_descriptor {
   
   uint16_t limit;      // bit 0-15, segment limit
   
   uint16_t baseLow;    // bit 0-23, base address 
   uint8_t baseMid;
   
   uint8_t flag1;       // descriptor flags 
   uint8_t flag2;       
   
   uint8_t baseHigh;   // base address high
} __attribute__((packed)) ;                                 //must be packed

//gdt flag bit access

#define GDT_ACCESS       1            // bit 40, flag1 first bit
#define GDT_READWRITE    2            // bit 41, flag1 second bit
#define GDT_EXP_DIR      4            // bit 42, flag1 third bit
#define GDT_CODE_SEG     8            // bit 43, flag1 4th bit
#define GDT_CODEDATA_DIS 16           // bit 44, flag1 5th bit
#define GDT_RING0        0            // bit 45-46, flag1 6-7 bit
#define GDT_RING1        32
#define GDT_RING2        64
#define GDT_RING3        65
#define GDT_SEG_MEM      128          // bit 47, flag1 8th bit

                                      // bit 48-51  part of limit, flag2 1-4 bit
#define GDT_ROS          16           // bit 52, flag2 5th bit
#define GDT_RES          32           // bit 53, flag2 6th bit
#define GDT_32BIT        64           // bit 54, flag2 7th bit
#define GDT_GRAN         128          // bit 55, flag2 8th bit

// set gdt descriptor table
extern void gdt_set_descriptor (uint32_t i, uint64_t base, uint64_t limit, uint8_t flag1, uint8_t flag2);

// return desciptor
extern struct gdt_descriptor* gdt_get_descriptor (int i);

//initialize gdt table
extern void gdt_initialize ();

#endif

