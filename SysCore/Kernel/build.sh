nasm -f elf loader.asm

gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -c -o kmain.o kmain.c

ld -m elf_i386 -T link.ld -o KERNEL.ELF loader.o kmain.o

objcopy -O binary KERNEL.ELF KERNEL.SYS

sudo cp KERNEL.SYS ../../myfloppy

