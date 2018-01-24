;*******************************
;    stdio.inc
;     - Input/Output routines
;*******************************
%ifndef __STDIO_INC_sneh_INCLUDED
%define __STDIO_INC_sneh_INCLUDED

; Puts16() - print a null terminated string, in real mode
; DS:SI -> 0 terminated string

bits 16

Puts16 : 
    pusha
    .loop1 : 
        lodsb                                   ; load next byte in al
        or al, al
        jz .exit                                ; end of string reached
        mov ah, 0eh
        int 10h
        jmp .loop1
    
    .exit : 
        popa
        ret

bits 32

%define VIDMEM 0xB8000                          ; address of video memory
%define COLS   80                               ; width of MODE 7 screen
%define LINES  25                               ; length of MODE 7 screen
%define CHAR_ATTR 36                            ; arrtibute associated

_CurX db 0                                      ; current position
_CurY db 0


;Putch32() - prints a character in 32 bit mode
;bl -> character to print
;****************************

Putch32 : 
   pusha                                        ; save registers
   mov edi, VIDMEM                              ; location of video memory
   
   ;calulate linear position in video memory => VIDMEM + (_CurX*2 + _CurY*COLS*2)
   xor eax, eax
   mov eax, COLS
   shl eax, 1                                   ; multiply by 2
   mov cl, byte [_CurY]
   mul cl
   
   xor ecx, ecx
   mov cl, byte[_CurX]
   shl ecx, 1                                   ; multiply by 2
   add eax, ecx
   add eax, edi                                 ; now eax contains exact linear position in video memory
   
   cmp bl, 0x0A                                 ; if this char is new line
   je .ROW
    
   mov bh, CHAR_ATTR                            ; bl contains character to write, bh contains its attribute
   mov word[eax], bx
   inc byte[_CurX]
   
   cmp byte[_CurX], COLS
   je .ROW
   jmp .done
   
   .ROW:
      mov byte[_CurX], 0
      inc byte[_CurY]
   
   .done:
      popa
      ret

;Puts32() - prints a string in 32 bit mode
;ebx -> null terminated string to print
;****************************************

Puts32 :
   pusha
   mov edi, ebx                                 ; copying string addess
   
   .loop :
      cmp byte[edi], 0x0                        ; end of string
      je .done
      
      mov bl, byte[edi]                         ; print this char
      call Putch32
      inc edi                                   ; next character
      jmp .loop
      
   .done :
      mov bh, byte[_CurY]                       ; prepare for MovCur call
      mov bl, byte[_CurX]
      call MovCur
      popa
      ret

;MovCur()   - moves cursor position
;bl -> x position
;bh -> y position
;*****************************************

MovCur :
   pusha                                        ; save register
   
   ;calculate current position => x + y*COLS
   xor eax, eax
   mov cl, COLS
   mov al, bh
   mul cl
   add al, bl                 
   mov ebx, eax                                 ; now eax contains current position
   
   mov al, 0x0f                                 ; cursur location low byte in index register
   mov dx, 0x03D4                               ; index register
   out dx, al
   
   mov al, bl                                   ; current position low byte
   mov dx, 0x03D5                               ; data register
   out dx, al
   
   mov al, 0x0e                                 ; cursur position high byte in index register
   mov dx, 0x03D4                               ; index register
   out dx, al
   
   mov al, bh                                   ; current position high byte
   mov dx, 0x03D5                               ; data register
   out dx, al
   
   popa
   ret
   
;ClrSrc32() - clears the screen
;******************************

ClrSrc32 :
   pusha
   cld
   mov edi, VIDMEM
   mov cx, 2000
   mov ah, CHAR_ATTR
   mov al, ' '
   rep stosw
   
   mov byte[_CurX], 0
   mov byte[_CurY], 0
   
   mov bl, 0                                    ; prepare for MovCur
   mov bh, 0
   call MovCur
   
   popa
   ret
   
%endif                                          ; __STDIO_INC_sneh_INCLUDED
