bits 16              ; we are in real mode

org  0x500           ; we are loaded at this address(0x050:0x00 = 0x500)

jmp main
   
    %include "Stdio.inc"
    %include "Gdt.inc"
    %include "A20.inc"
    %include "Fat12.inc"
    %include "common.inc"
   
; This is stage 2 boot_loader, its task includes
; + Store BIOS information
; + Load Kernel
; + Install GDT; switch to protected mode
; + jump to stage 3

LoadingMsg db "Preparing to load Operating system.....", 0x0D, 0x0A, 0x0
msgFailure db 0x0D, 0x0A, "*** FATAL: MISSING OR CURRUPT KRNL.SYS. Press Any Key to Reboot", 0x0D, 0x0A, 0x0A, 0x00
ProtectedModeOn db "Welcome to my Operating System", 0x0A, 0x0
	
main:
     ;setup SEGMENT (remember segment:offset) and stack first in real mode
     cli
     mov ax, 0
     mov ds, ax
     mov es, ax
     mov ax, 0x9000          ; let our stack begin here
     mov ss, ax              ; stack segment
     mov sp, 0xFFFF          ; let our stack pointer be pointing to at 0X9000:0xFFFF in memory
     sti
    
     ;print loading message
     mov si, LoadingMsg
     call Puts16             ; defined in Stdio.inc
     
     ;install GDT
     call InstallGDT         ; defined in Gdt.inc, *now all memory refrence from gdt* address 
    
     ;enable A20
     call keyboardControllerOutputEnableA20     ; now we can access upto 4GB 
     
     ; before swithing to protected mode load kernel 
     
     ; load root table
     call LoadRoot            ; Load root directory table
     
     ; load kernel
     mov ebx, 0               ; bx:bp points to buffer to load to
     mov bp, IMAGE_RMODE_BASE
     mov si, ImageName        ; our file to load
     call LoadFile
     
     mov dword[ImageSize], ecx ; save size of kernel
     cmp ax, 0                 ; test for success
     je Stage3
     
     mov si, msgFailure
     call Puts16
     mov ah, 0
     int 0x16                  ; wait for key press
     int 0x19                  ; restart
     cli
     hlt
     
     Stage3 :
     ;switch to protected mode
     cli 
     mov eax, cr0            ; set bit 0 in cr0 --> entering in Pmode
     or  eax, 1
     mov cr0, eax
    
     jmp CODE_DESC:PrepareKernelLoad          ; far jump to fix cs, 08h is address of gdt code sector (refrence from gdt address)
    
     ;before enabling interrupt setup ds, cs, ss, esp
    
     bits 32
     
     PrepareKernelLoad :
         mov ax, DATA_DESC        ; data segment to data selecter
         mov ds, ax
         mov ss, ax          ; stack segment to stack selecter
         mov es, ax
         mov esp, 90000h    ; let stack pointer initially be pointing to 90000h in memory
         
         ;copy kernel image at 1MB in memory
     CopyImage:
         mov eax, dword [ImageSize]
         movzx ebx, word[bpbBytesPerSector]     ; zero extend
         mul ebx
         mov ebx, 4                             ; we will be moving 4-4 bytes
         div ebx
         cld                  ; clear direction flag
         mov esi, IMAGE_RMODE_BASE
         mov edi, IMAGE_PMODE_BASE
         mov ecx, eax
         rep movsd            ; copy image at 1MB
         
         ; print protected mode on
         call ClrSrc32
         mov byte[_CurX], 25
         mov byte[_CurY], 2
         mov bl, 10
         mov bh, 2
         call MovCur
         
         mov ebx, ProtectedModeOn      ;print string in protected mode
         call Puts32
         
         ; execute kernel
         xor edx, edx
         mov edx, 24
         add edx, IMAGE_PMODE_BASE
         mov ebx, dword[edx]
         ;add ebx, IMAGE_PMODE_BASE 
         call edx
         
     STOP:
     cli
     hlt
