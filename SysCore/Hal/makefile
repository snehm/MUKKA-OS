CC = gcc
CCFLAG = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -c -I../Include

LD = ld
LDFLAG = -m elf_i386 -r

HAL.o: cpu.o gdt.o idt.o pic.o pit.o pit_handler_wrapper.o halTemp.o
	 $(LD) $(LDFLAG) cpu.o gdt.o idt.o pic.o pit.o pit_handler_wrapper.o halTemp.o -o HAL.o

cpu.o: cpu.c
	$(CC) $(CCFLAG) cpu.c -o cpu.o
	
gdt.o: gdt.c
	$(CC) $(CCFLAG) gdt.c -o gdt.o

idt.o: idt.c
	$(CC) $(CCFLAG) idt.c -o idt.o

pic.o: pic.c
	$(CC) $(CCFLAG) pic.c -o pic.o

pit.o: pit.c
	$(CC) $(CCFLAG) pit.c -o pit.o

halTemp.o: hal.c
	$(CC) $(CCFLAG) hal.c -o halTemp.o

pit_handler_wrapper.o: pit_handler_wrapper.s
	nasm -f elf pit_handler_wrapper.s -o pit_handler_wrapper.o 

clean:
	-rm *.o
	
