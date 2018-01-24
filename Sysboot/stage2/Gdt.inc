;********************************
; Gdt.inc
;   - GDT table definition
;********************************

%ifndef __GDT_INC_sneh_INCLUDED
%define __GDT_INC_sneh_INCLUDED

bits 16                         ; we will be in 16 bit mode when loading gdt

; InstallGDT()
;    - installing this gdt

InstallGDT:
    cli 
    pusha
    lgdt [toc]                  ; load gdt in GDTR
    sti
    popa
    ret

; GDT table
gdt_data:
    dd 0
    dd 0                        ;null descripter
    
    ;gdt for code segment
    dw 0FFFFh                   ;gdt table, refer gdt table
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
    
    ;gdt for data segment
    dw 0FFFFh                   ;gdt table, refer gdt table
    dw 0
    db 0
    db 10010010b                ;becuase this is data segment
    db 11001111b
    db 0
end_of_gdt:

toc: 
    dw end_of_gdt - gdt_data - 1   ; size of gdt table
    dd gdt_data                    ; must be in this form
   
%define NULL_DESC 0
%define CODE_DESC 0x8
%define DATA_DESC 0x10
    
%endif ;__GDT_INC_sneh_INCLUDED
