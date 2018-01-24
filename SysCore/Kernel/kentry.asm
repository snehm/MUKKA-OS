; build :: nasm -f elf loader.asm

; This will be kernel's entry point from boot loader
 
[BITS 32]
[GLOBAL start]
[EXTERN kmain]

SECTION .text

start: 
   
   cli            
	mov ax, 10h                               ; setup discriptors for kernel
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
		
   call kmain                                ; call kernel
   
   cli
   hlt
