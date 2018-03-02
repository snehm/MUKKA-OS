#include <hal.h>
#include "pic.h"
#include <stdint.h>

//PIC I registers
#define I86_PIC1_REG_COMMAND              0x20           // command register
#define I86_PIC1_REG_STATUS               0x20           // status register
#define I86_PIC1_REG_DATA                 0x21           // data register
#define I86_PIC1_REG_IMR                  0x21           // interrupt mask register (IMR)

//PIC II register
#define I86_PIC2_REG_COMMAND              0xA0
#define I86_PIC2_REG_STATUS               0xA0
#define I86_PIC2_REG_DATA                 0xA1
#define I86_PIC2_REG_IMR                  0xA1


//Initialization word I MASK
#define I86_PIC_ICW1_MASK_IC4             0x1            // 00000001 -> expect ICW4
#define I86_PIC_ICW1_MASK_SNGL            0x2            // 00000010 -> single or cascaded
#define I86_PIC_ICW1_MASK_ADI             0x4            // 00000100 -> call address interval
#define I86_PIC_ICW1_MASK_LTIM            0x8            // 00001000 -> operation mode
#define I86_PIC_ICW1_MASK_INIT            0x10           // 00010000 -> initialization command

//Initailization word I -> achitecture information
#define I86_PIC_ICW1_IC4_EXPECT           1              // 1
#define I86_PIC_ICW1_IC4_NO               0              // 0
#define I86_PIC_ICW1_SNGL_YES             2
#define I86_PIC_ICW1_SNGL_NO              0
#define I86_PIC_ICW1_ADI_CALLINTERVAL4    4
#define I86_PIC_ICW1_ADI_CALLINTERVAL8    0
#define I86_PIC_ICW1_LTIM_LEVELTRIGGERED  8
#define I86_PIC_ICW1_LTIM_EDGETRIGGERED   0
#define I86_PIC_ICW1_INIT_YES             0x10
#define I86_PIC_ICW1_INIT_NO              0


//Initialization word II -> IVT Information

//Initialization word III -> cascadind inforamation

//Initailization word IV MASK
#define I86_PIC_ICW4_MASK_UPM             0x1            // 00000001 -> mode of operation
#define I86_PIC_ICW4_MASK_AEOI            0x2            // 00000010 -> automatic EOI
#define I86_PIC_ICW4_MASK_MS              0x4            // 00000100 -> select buffer type
#define I86_PIC_ICW4_MASK_BUF             0x8            // 00001000 -> buffered mode
#define I86_PIC_ICW4_MASK_SFNM            010            // 00010000 -> special fully nested mode

//Initailizataion word IV -> mode of operation information
#define I86_PIC_ICW4_UPM_86MODE           1          
#define I86_PIC_ICW4_UPM_MCSMODE          0
#define I86_PIC_ICW4_AEOI_AUTOAEOI        2
#define I86_PIC_ICW4_AEOI_NOAUTOAEOI      0
#define I86_PIC_ICW4_MS_BUFFERMASTER      4
#define I86_PIC_ICW4_MS_BUFFERSLAVE       0
#define I86_PIC_ICW4_BUF_MODEYES          8
#define I86_PIC_ICW4_BUF_MODENO           0
#define I86_PIC_ICW4_SFNM_NESTEDMODE      0x10
#define I86_PIC_ICW4_SFNM_NONESTED        0

//send command to PIC
inline void i86_pic_send_command( uint8_t cmd, uint8_t picNum ) {
   if(picNum > 1)
      return;

   uint8_t reg =  (picNum == 1) ? I86_PIC2_REG_COMMAND : I86_PIC1_REG_COMMAND ;
   outportb(reg, cmd);
}

//send data to PIC
inline void i86_pic_send_data( uint8_t data, uint8_t picNum ) {
   if(picNum > 1)
      return;

   uint8_t reg =  (picNum == 1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA ;
   outportb(reg, data);
}

//read data from PIC
inline uint8_t i86_pic_read_data( uint8_t picNum ) {
   if(picNum > 1)
      return 0;

   uint8_t reg = (picNum == 1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA ;
   return inportb(reg);
}

//PIC initialization
void i86_pic_initialize (uint8_t base0, uint8_t base1) {

   uint8_t icw = 0;

   //diable hardware interrupt
   disable ();

   //prepare ICW1
   icw = (icw & ~I86_PIC_ICW1_MASK_INIT ) | I86_PIC_ICW1_INIT_YES;
   icw = (icw & ~I86_PIC_ICW1_MASK_IC4 ) | I86_PIC_ICW1_IC4_EXPECT;

   //send ICW1 to PIC(s)
   i86_pic_send_command (icw, 0);
   i86_pic_send_command (icw, 1);

   //send ICW2
   i86_pic_send_data (base0, 0);
   i86_pic_send_data (base1, 1);

   //send ICW3
   i86_pic_send_data (0x04, 0);
   i86_pic_send_data (0x02, 1);

   //prepare ICW 4
   icw = 0;
   icw = (icw & ~I86_PIC_ICW4_MASK_UPM ) | I86_PIC_ICW4_UPM_86MODE;

   //send ICW4
   i86_pic_send_data (icw, 0);
   i86_pic_send_data (icw, 1);


   //diable all hardware interrupt except clock
   i86_pic_send_data (254, 0);
   i86_pic_send_data (255, 1);
}