; Fat12.inc
;   - FAT12 filesystem for 3-1/2 floppies
;*************************************************

%ifndef __FAT12_INC_sneh_INCLUDED__
%define __FAT12_INC_sneh_INCLUDED__

bits 16

%include "Floppy16.inc"                         ; entaract with floppy in real mode
%include "Stdio.inc"

%define ROOT_OFFSET 0x2e00
%define FAT_SEG 0x2e0
%define ROOT_SEG 0x2e0

;LoadRoot()
;   - Load Root Directory table to 0x2e00
;*****************************************

msg db ">#here",0x0

LoadRoot:
    pusha
    push es
    
    ;size of root directory to read => 32 * no of root directories in root
    
    xor cx, cx
    xor dx, dx
    mov ax, 32
    mul word[bpbRootEntries]
    div word[bpbBytesPerSector]
    xchg ax, cx
    
    ;location of root directory => total sectors for FATS + Reserved sectors
    
    mov al, byte[bpbNumberOfFATs]   
    mul word[bpbSectorsPerFAT]
    add ax, word[bpbReservedSectors]
    mov word[datasector], ax                    ; start base of root directory
    add word[datasector], cx                    ; end sector of root directory
    
    ;read root directory to 0x2e00
    
    push word ROOT_SEG
    pop es
    mov bx, 0
   
    call ReadSectors
     
    pop es
    popa
    ret

; LoadFAT()
;       - Loads FAT table at 0x2c00
;       es:di => root directory table
;*************************************

LoadFAT:
    
    pusha
    push es
    
    ;size of FAT in cx = no of FAT * sectors per FAT
    xor ax, ax
    mov al, byte[bpbNumberOfFATs]
    mul word[bpbSectorsPerFAT]
    mov cx, ax
    
    mov ax, word[bpbReservedSectors]
    
    ; read FAT
    push word FAT_SEG
    pop es
    xor bx, bx
    call ReadSectors
    pop es
    popa
    ret
    
;FindFile()
;   - Search for filename in root table
; ds:si => file name
; ret ax => file index number in root table or -1

FindFile:
    push cx
    push dx
    push bx
    mov bx, si                               ; file name copy
    
    mov cx, word[bpbRootEntries]             ; loop counter
    mov di, ROOT_OFFSET
    cld                                      ; clear direction flag
    
    .LOOP :
        push cx
        mov cx, 11                           ; 11 byte name
        mov si, bx
        push di
        rep cmpsb
        pop di
        je .Found
        pop cx
        add di, 32                           ; next root table entry
        loop .LOOP
    
    .NotFound:
        pop bx
        pop dx
        pop cx
        mov ax, -1
        ret
    
    .Found :
        pop ax                               ; prepare for return 
        pop bx
        pop dx
        pop cx
        ret

; LoadFile()
;       - loads a file
;       es:si => file to load
;       ebx:bp => buffer to load file
;       ax => 0 on success, or -1
;       cx => number of sectors to read
;*******************************************

LoadFile:
    
    xor ecx, ecx
    push ecx
    
    .FIND_FILE:
        push bx
        push bp
        call FindFile
        cmp ax, -1
        
        jne .LOAD_IMAGE_PRE
        pop bp
        pop bx
        pop ecx
        mov ax, -1
        ret
    
    .LOAD_IMAGE_PRE:
    
        sub edi, ROOT_OFFSET
        sub eax, ROOT_OFFSET
        
        ; starting cluster
        push word ROOT_SEG
        pop es
        mov dx, word[es:di + 0x001A]         ; get first cluster information
        mov word[cluster], dx           
        pop bx
        pop es
        push bx                              ; location for later use
        push es
        call LoadFAT
        
     
    .LOAD_IMAGE:

	; load cluster

	mov	ax, WORD [cluster]	               ; cluster to read
	pop	es			; bx:bp=es:bx
	pop	bx
	call	ClusterLBA
	xor	cx, cx
	mov     cl, BYTE [bpbSectorsPerCluster]
	call	ReadSectors
	pop	ecx
	inc	ecx			                        ; add one more sector to counter
	push	ecx
	push	bx
	push	es
	mov	ax, FAT_SEG		                     ;start reading from fat
	mov	es, ax
	xor	bx, bx

	; get next cluster

	mov     ax, WORD [cluster]	               ; identify current cluster
	mov     cx, ax			                     ; copy current cluster
	mov     dx, ax
	shr     dx, 0x0001		                  ; divide by two
	add     cx, dx			                     ; sum for (3/2)

	mov	bx, 0			                        ;location of fat in memory
	add	bx, cx
	mov	dx, WORD [es:bx]
	test	ax, 0x0001		                     ; test for odd or even cluster
	jnz	.ODD_CLUSTER

    .EVEN_CLUSTER:

	and	dx, 0000111111111111b	            ; take low 12 bits
	jmp	.DONE

    .ODD_CLUSTER:

	shr	dx, 0x0004		                     ; take high 12 bits

    .DONE:

	mov	WORD [cluster], dx
	cmp	dx, 0x0ff0		                     ; test for end of file marker
	jb	.LOAD_IMAGE

    .SUCCESS:
	pop	es
	pop	bx
	pop	ecx
	xor	ax, ax
	ret

%endif
        
    
    
     
