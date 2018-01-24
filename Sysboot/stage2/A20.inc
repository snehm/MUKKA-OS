;*************************************************
;       A20.inc
;           -method for enabling address line 20
;*************************************************

%ifndef __A20_INC_sneh_INCLUDED
%define __A20_INC_sneh_INCLUDED

;method 1 -> less poratable, System control 0x92

systemControlEnableA20 :
    
    cli
    push ax
    mov al, 2                 ; set bit 2 (enable A20)
    out 0x92, al
    pop ax
    sti
    ret

;method 2 -> bios support
;return 
;CF = 0, if success
;Ah = 0, if success

biosEnableA20 :
    push ax
    mov ax, 0x2401
    int 0x15
    pop ax
    ret

biosDisableA20 :
    push ax
    mov ax, 0x2400
    int 0x15
    pop ax
    ret

;method 3 -> keyboard controller

keyboardControllerEnableA20 :
    
    cli
    push ax
    mov al, 0xdd              ; command 0xdd : enable A20
    out 0x64, al              ; send command
    pop ax
    sti
    ret

keyboardControllerDisableA20 :
    
    cli
    push ax
    mov al, 0xdf              ; command 0xdf : disable A20
    out 0x64, al
    pop ax
    sti
    ret

;method 3.2 -> keyboard controller through output port, see text

keyboardControllerOutputEnableA20 :
   
  cli
	pusha

        call    wait_input
        mov     al,0xAD
        out     0x64,al		   ; disable keyboard, tell 0x64 this is your data (command)
        call    wait_input

        mov     al,0xD0
        out     0x64,al		   ; read output port, tell 0x64 this is your data (command)
        call    wait_output

        in      al,0x60 
        push    eax		      ; get output port data and store it
        call    wait_input

        mov     al,0xD1
        out     0x64,al		   ; tell controller to write output port
        call    wait_input

        pop     eax
        or      al,2		      ; set bit 1 (enable a20)
        out     0x60,al		   ; write out data back to the output port

        call    wait_input
        mov     al,0xAE		   ; enable keyboard
        out     0x64,al

        call    wait_input
	popa
        sti
        ret

	; wait for input buffer to be clear

wait_input:                   ; i am busy executing previous cmds
        in      al,0x64       ; this return its status
        test    al,2
        jnz     wait_input
        ret

	; wait for output buffer to be clear

wait_output:
        in      al,0x64
        test    al,1
        jz      wait_output
        ret
   

%endif
