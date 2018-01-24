;Floppy16.inc
;      - Floppy drive interaction in real mode
;***************************************************

%ifndef __FLOPPY16_INC_sneh_INCLUDED__
%define __FLOPPY16_INC_sneh_INCLUDED__

bits 16

bpbOEM			         db "My OS   "
bpbBytesPerSector:  	   DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	   DW 1
bpbNumberOfFATs: 	      DB 2
bpbRootEntries: 	      DW 224
bpbTotalSectors: 	      DW 2880
bpbMedia: 	            DB 0xf0                 ;; 0xF1
bpbSectorsPerFAT: 	   DW 9
bpbSectorsPerTrack: 	   DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	   DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	      DB 0
bsUnused: 		         DB 0
bsExtBootSignature: 	   DB 0x29
bsSerialNumber:	      DD 0xa0a1a2a3
bsVolumeLabel: 	      DB "MOS FLOPPY "
bsFileSystem: 	         DB "FAT12   "

datasector dw 0x0000
cluster    dw 0x0000

absoluteSector db 0x00
absoluteHead   db 0x00
absoluteTrack  db 0x00

; convers CHS to ClusterLBA
; LBA = (cluster - 2) * sectors per cluster
; ax -> cluster number
; -2 because 2 clusters are reserved, cluster numbering starts from 2
;***************************************************************************


ClusterLBA :
    sub ax, 0x0002
    xor cx, cx
    mov cl, byte [bpbSectorsPerCluster]
    mul cx
    add ax, word [datasector]                           ; base of data sector in FAT12 floppy
    ret 

; convert LBA to CHS
; AX => LBA address to convert
; absolute sector = (logical sector % sectors per track) + 1
; assolute head   = (logical sector / sectors per track) MOD number of head
; absolute track  = logical sector / (sectors per track * number of heads)

LBACHS : 
    xor dx, dx
    div word [bpbSectorsPerTrack]       
    inc dl
    mov byte [absoluteSector], dl
    xor dx, dx
    div word [bpbHeadsPerCylinder]
    mov byte [absoluteHead], dl
    mov byte [absoluteTrack],al
    ret
    
; read a series of sector
; cx => Number of sectors to read
; Ax => starting sector
; ES:EBX => Buffer to read to

ReadSectors :
    
    .MAIN :
        mov di, 0x0005                                  ; we will try 5 times if failed
    .SECTORLOOP :
        push ax
        push bx
        push cx
        call LBACHS                                     ; we will get absolute sector, absolute head and absolute track information from here
        mov ah, 0x02                                    ; prepare bios read interrupt
        mov al, 0x01                                    ; we will read one sector
        mov ch, byte[absoluteTrack]
        mov cl, byte[absoluteSector]
        mov dh, byte[absoluteHead]
        mov dl, byte[bsDriveNumber]
        int 0x13                                        ; bios read interrupt call
        jnc .SUCCESS                                    ; if sucessful
        xor ax, ax                                      ; retying again
        int 0x13                                        ; reset disk interuppt call
        dec di                                          ; decrease a try
        pop cx
        pop bx
        pop ax
        jnz .SECTORLOOP
        int 0x18                                        ; start basic, failed => give control to bios
        
        .SUCCESS:
            pop cx
            pop bx
            pop ax
            add bx, word[bpbBytesPerSector]             ; next read buffer location
            inc ax                                      ; next sector to read
            loop .MAIN
            ret
        
%endif
