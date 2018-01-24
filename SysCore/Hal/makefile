CC = gcc
CCFLAG = -m32 -masm=intel -nostdlib -nostdinc -fno-builtin -fno-stack-protector -c -I../Include

LD = ld
LDFLAG = -m elf_i386 -r

HAL.o: cpu.o gdt.o idt.o halTemp.o
	 $(LD) $(LDFLAG) cpu.o gdt.o idt.o halTemp.o -o HAL.o

cpu.o: cpu.c
	$(CC) $(CCFLAG) cpu.c -o cpu.o
	
gdt.o: gdt.c
	$(CC) $(CCFLAG) gdt.c -o gdt.o

idt.o: idt.c
	$(CC) $(CCFLAG) idt.c -o idt.o

halTemp.o: hal.c
	$(CC) $(CCFLAG) hal.c -o halTemp.o

clean:
	-rm *.o
	
