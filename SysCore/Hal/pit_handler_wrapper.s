extern i86_pit_irq
global i86_pic_handler_wrapper

i86_pic_handler_wrapper:
   cli
   pusha

   call i86_pit_irq

   popa
   sti
   iret