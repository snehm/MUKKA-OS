CC = gcc
AS = nasm
ASFLAG = -f bin 

LOADER.SYS : boot1.asm
	$(AS) $(ASFLAG) boot1.asm -o "LOADER.SYS"

clean:
	-rm *.SYS

install:
	sudo dd if=LOADER.SYS of=/dev/loop0
