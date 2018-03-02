/*
   * gdt.c implements interfaces provided by gdt.h
   * compile with gcc -masm=intel
*/

#include "gdt.h"
#include <_null.h>
#include <stdint.h>
#include <string.h>

//gdtr, to install gdt
struct gdtr {
   
   //size of GDT
   uint16_t size;
   
   //base address of GDT
   uint32_t base;
} __attribute__((packed)) ;            //must be packed


//GDT table
static struct gdt_descriptor _gdt [GDTMAX];

//gdtr data
static struct gdtr _gdtr;

//install gdt in processor
static void install_gdt(){
   asm ("lgdt (%0)" : : "r" (&_gdtr));
}

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t flag1, uint8_t flag2) {
   
   if(i >= GDTMAX)
      return;
      
   memset((void *)&_gdt[i], 0, sizeof(struct gdt_descriptor));
   
   _gdt[i].baseLow = (uint16_t) (base & 0xffff );
   _gdt[i].baseMid = (uint8_t)( (base >> 16 ) & 0xff );
   _gdt[i].baseHigh = (uint8_t) ( (base >> 24 ) & 0xff );
   
   _gdt[i].limit = (uint16_t) ( ( limit & 0xffff ) );
   
   _gdt[i].flag1 = (uint8_t) ( flag1 & 0xff );
   _gdt[i].flag2 = (uint8_t) ( (limit >> 16) & 0x0f);
   _gdt[i].flag2 |= flag2 & 0xf0 ;
}

struct gdt_descriptor* gdt_get_descriptor(int i){
   if(i >= GDTMAX)
      return NULL;
   
   return &_gdt[i];
}

void gdt_initialize(){
   _gdtr.size = (uint16_t)(sizeof(struct gdt_descriptor) * GDTMAX ) -1;  // -1 is in rule of gdtr
   _gdtr.base = (uint32_t) &_gdt[0];
   
   //lets set null descriptor
   gdt_set_descriptor(0, 0, 0, 0, 0);
   
   //set default code descriptor to 11111111 11111111 00000000 00000000 00000000 10011010 11001111 00000000
   gdt_set_descriptor(1, 0, 0xffffffff, GDT_READWRITE|GDT_CODE_SEG|GDT_CODEDATA_DIS|GDT_RING0|GDT_SEG_MEM, 
                      GDT_32BIT|GDT_GRAN);
   
   //set default data descriptor to 11111111 11111111 00000000 00000000 00000000 10010010 11001111 00000000
   gdt_set_descriptor(2, 0, 0xffffffff, GDT_READWRITE|GDT_CODEDATA_DIS|GDT_RING0|GDT_SEG_MEM, 
                      GDT_32BIT|GDT_GRAN);
   
   //install gdtr
   install_gdt();
}

