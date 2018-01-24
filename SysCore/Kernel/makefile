AS = nasm
ASFLAG = -f elf

CC = gcc
CCFLAG = -m32 -masm=intel -nostdlib -nostdinc -fno-builtin -fno-stack-protector -c -I../Include

LD = ld
LDFLAG = -m elf_i386 -T link.ld

KERNEL.SYS: kmain.o kentry.o kprint.o
	$(LD) $(LDFLAG) -o KERNEL.ELF kentry.o kmain.o kprint.o "../Lib/mylib.o" "../Hal/HAL.o"
	objcopy -O binary KERNEL.ELF KERNEL.SYS
	
kprint.o : kprint.c
	$(CC) $(CCFLAG) -o kprint.o kprint.c
	
kmain.o: kmain.c
	$(CC) $(CCFLAG) -o kmain.o kmain.c
	
kentry.o: kentry.asm
	$(AS) $(ASFLAG) kentry.asm

clean:
	-rm *.o *.ELF *.SYS
install:
	sudo cp KERNEL.SYS ../../myfloppy
