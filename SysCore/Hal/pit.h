#ifndef _PIT_H
#define _PIT_H

#include <stdint.h>

//operation command word masks -> initiliaze counter
#define I86_PIT_OCW_MASK_BITCOUNT      1     // 00000001 -> binary or bcd counter
#define I86_PIT_OCW_MASK_MODE          0xE   // 00001110 -> operation mode
#define I86_PIT_OCW_MASK_RL            0x30  // 00110000 -> read/load mode
#define I86_PIT_OCW_MASK_COUNTER       0xC0  // 11000000 -> counter number

//operational commands
#define I86_PIT_OCW_BINCOUNT_BINARY    0     // 0
#define I86_PIT_OCW_BINCOUNT_BCD       1     // 1
#define I86_PIT_OCW_MODE_TERMINALCOUNT 0     // 0000
#define I86_PIT_OCW_MODE_ONESHOT       0x2   // 0010
#define I86_PIT_OCW_MODE_RATEGEN       0x4   // 0100
#define I86_PIT_OCW_MODE_SQUAREWAVEGEN 0x6   // 0110
#define I86_PIT_OCW_MODE_SOFTWARETRIG  0x8   // 1000
#define I86_PIT_OCW_MODE_HARDWARETRIG  0xA   // 1010
#define I86_PIT_OCW_RL_LATCH           0     // 000000
#define I86_PIT_OCW_RL_LSBONY          0x10  // 010000
#define I86_PIT_OCW_RL_MSBONY          0x20  // 100000
#define I86_PIT_OCW_RL_DATA            0x30  // 110000
#define I86_PIT_OCW_COUNTER_0          0     // 00000000
#define I86_PIT_OCW_COUNTER_1          0x40  // 01000000
#define I86_PIT_OCW_COUNTER_2          0x80  // 10000000

//pit send command
extern void i86_pit_send_command (uint8_t cmd);

//pit send data
extern void i86_pit_send_data (uint16_t data, uint8_t counter);

//read data from pit
extern uint8_t i86_pit_read_data ( uint16_t counter);

// initialize pit
extern void i86_pit_initialize (uint8_t irq);

//start pic
extern void i86_pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode);

//get tick count
extern uint32_t i86_pit_get_tick_count ();

//set tick count
extern uint32_t i86_pit_set_tick_count (uint32_t x);

//isr wrapper function
extern void i86_pic_handler_wrapper(void);

#endif