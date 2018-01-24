#ifndef _HAL_H
#define _HAL_H

//initiaize Hardware abstraction layer
extern int hal_initialize();

//shutdown Hardware abstraction layer
extern int hal_shutdown();

//generate interruppt
extern void generate_interrupt(int i);

#endif
