CC = gcc
CCFLAG = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -c -I../Include

LD = ld
LDFLAG = -m elf_i386

mylib.o:
	$(CC) $(CCFLAG) -o mylib.o string.c

clean:
	-rm *.o  
