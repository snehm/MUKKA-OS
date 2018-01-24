bits 16

org  0                                             ; we will set segments later

start:	jmp	main

;***** BPB Parameter block *****
;must start 3 bytes after start (jmp main is of 3 byte)

bpbOEM			      db "My OS   "			         ; OEM identifier (Cannot exceed 8 bytes!)
bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster:DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	   DB 2
bpbRootEntries: 	   DW 224
bpbTotalSectors: 	   DW 2880
bpbMedia: 		      DB 0xf8                       ;; 0xF1
bpbSectorsPerFAT: 	DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: DW 2
bpbHiddenSectors: 	DD 0
bpbTotalSectorsBig:  DD 0
bsDriveNumber: 	   DB 0
bsUnused: 		      DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	   DD 0xa0a1a2a3
bsVolumeLabel: 	   DB "MOS FLOPPY "
bsFileSystem: 	      DB "FAT12   "

Print:
    ;string pointed by ds:si, terminated by 0
    lodsb                                          ; load next byte pointed by si to al
    or al, al                                      ; if al is zero
    jz PrintDone
    mov ah, 0eh                                    ; prepare fro bios vedio interuppt call
    int 10h
    jmp Print
    PrintDone:
        ret

ReadSectors :
    ;cx = no fo sectors to ReadSectors
    ;ax = strating sectors
    ;ES:BX = buffer to read to
    
    .MAIN :
        mov di, 0x0005                             ; we will try 5 times if failed
    .SECTORLOOP :
        push ax
        push bx
        push cx
        call LBACHS                                ; we will get absolute sector, absolute head and absolute track information from here
        mov ah, 0x02                               ; prepare bios read interrupt
        mov al, 0x01                               ; we will read one sector
        mov ch, byte[absoluteTrack]
        mov cl, byte[absoluteSector]
        mov dh, byte[absoluteHead]
        mov dl, byte[bsDriveNumber]
        int 0x13                                   ; bios read interrupt call
        jnc .SUCCESS                               ; if sucessful
        xor ax, ax                                 ; retying again
        int 0x13                                   ; reset disk interuppt call
        dec di                                     ; decrease a try
        pop cx
        pop bx
        pop ax
        jnz .SECTORLOOP
        int 0x18                                    ; start basic, failed => give control to bios
        
        .SUCCESS:
            pop cx
            pop bx
            pop ax
            add bx, word[bpbBytesPerSector]         ; next read buffer location
            inc ax                                  ; next sector to read
            loop .MAIN
            ret

ClusterLBA:
;convert custer no to LBACHS
;LBA = (cluster - 2)*sectors per cluster
        sub ax, 0x0002
        xor cx, cx
        mov cl, byte[bpbSectorsPerCluster]
        mul cx
        add ax, word[datasector]                    ; base of data sectors
        ret
        
LBACHS:
;convert LBA to CHS
;absolute sector = (logical sector % sector per track) + 1
;absolute head = (logical sector / sectors per track) % no of heads
;absoluteTrack = logical sector / (sectors per track * no of heads)
        xor dx, dx  
        div word [bpbSectorsPerTrack]
        inc dl
        mov byte[absoluteSector], dl
        xor dx, dx
        div word[bpbHeadsPerCylinder]
        mov byte[absoluteHead], dl
        mov byte[absoluteTrack], al
        ret

main:
        cli
        mov ax, 0x07C0                             ; 0x7C0*2^4 = 0x7C00
        mov ds, ax                                 ; setup segment registers
        mov es, ax
        mov fs, ax
        mov gs, ax
        
        ;create stack
        mov ax, 0x0000
        mov ss, ax
        mov sp, 0xFFFF
        sti
        
        ;display Starting
        mov si, msgStart
        call Print
        
        ;load root directory table
        LOAD_ROOT:
            ;size of root directory = 32*(no of root entries)/(bytes per sector) SECTORS
            xor cx, cx
            xor dx, dx
            mov ax, 0x0020
            mul word[bpbRootEntries]
            div word[bpbBytesPerSector]
            xchg ax, cx                            ; size of root in cx
            
            ;location root directory = reserved sectors + (no of FATS * sectors per FAT)
            mov al, byte [bpbNumberOfFATs]
            mul word [bpbSectorsPerFAT]
            add ax, word[bpbReservedSectors]
            mov word [datasector], ax
            add word [datasector], cx              ; start of data sector
            
            ;read root directory into memory 0x7C0:0x0200 = 7C00+0200
            mov bx, 0x0200
            call ReadSectors
            
            ;find stage2 boot loader
            mov cx, word[bpbRootEntries]
            mov di, 0x0200                         ; address of root table in memory
            .LOOP:
                push cx
                mov cx, 0x000B                     ; 11 character name
                mov si, ImageName                  ; Image name
                push di
                rep cmpsb
                    pop di
                    je LOAD_FAT
                    pop cx
                    add di, 0x0020                 ; next entry in root table
                    loop .LOOP
                    jmp FAILURE
                    
            LOAD_FAT:
            ;save starting cluster of boot image
            mov dx, word[di+0x001A]
            mov word[cluster], dx
            
            ;compute size of FAT and store in cx = no of FAT * sectors per FAT
            xor ax, ax
            mov al, byte[bpbNumberOfFATs]
            mul word[bpbSectorsPerFAT]
            mov cx, ax
            
            ;location of FAT in ax = no of reserved Sectors
            mov ax, word[bpbReservedSectors]
            
            ;read FAT in memory at 0x7C0:0x0200
            mov bx, 0x0200
            call ReadSectors
            
            ;load image file into memory at 0050:0000 => 0x500)
            mov ax, 0x0050
            mov es, ax
            mov bx, 0x0000
            push bx
            
            LOAD_IMAGE:
          
            mov ax, word[cluster]
            pop bx
            call ClusterLBA
            xor cx, cx
            mov cl, byte[bpbSectorsPerCluster]
            call ReadSectors
            push bx
            
            ;compute next cluster, remember cluster entries are of 12 bits = 3/2 bytes
            mov ax, word[cluster]
            mov cx, ax
            mov dx, ax
            shr dx, 0x0001                         ; devide by 2
            add cx, dx                             ; equals to 3/2*cluster number
            mov bx, 0x0200                         ; location of FAT in memory
            add bx, cx                             ; we moved 3/2*cluster no bytes from start
            mov dx, word[bx]                       ; read 2 bytes in dx
            test ax, 0x0001                        ; if this was odd cluster =>  lower 12 bits, else =>higher 12 bits will be used
            jnz .ODD_CLUSTER
            
            .EVEN_CLUSTER:
                and dx, 0000111111111111b
                jmp .DONE
            
            .ODD_CLUSTER:
                shr dx, 0x0004
            
            .DONE:
                mov word[cluster], dx              ; new cluster location
                cmp dx, 0x0FF0                     ; if this was last cluster
                jb LOAD_IMAGE
            
            DONE:
                mov si, msgStageTwo
                call Print
                push word 0x0050                   ; segment address
                push word 0x0000                   ; offset in segment
                retf                               ; far return, both of the above value will be used
                
            FAILURE:
                mov si, msgFailure
                call Print
                mov ah, 0x00
                int 0x16                           ; wait for key press
                int 0x19                           ; reboot
                
            
            absoluteSector db 0x00
            absoluteHead   db 0x00
            absoluteTrack  db 0x00
            
            datasector     dw 0x0000
            cluster        dw 0x0000
            ImageName      db "KRNLDR  SYS"         ;must be 11 bytes
            msgStart     db 0x0D,0x0A, "Boot Loader starting (0x7c00)....", 0x0D, 0x0A, 0x00
            msgStageTwo  db "Jumping to stage2 (0x500)", 0x0D, 0x0A, 0x0D, 0x0A, 0x00
            msgFailure  db 0x0D, 0x0A, "ERROR : Press Key to Reboot", 0x0A, 0x00
            
            times 510-($-$$) db 0
            dw 0xAA55                              ; boot signature
