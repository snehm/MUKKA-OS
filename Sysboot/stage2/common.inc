%ifndef __COMMON_INC_INCLUDED
%define __COMMON_INC_INCLUDED

%define IMAGE_PMODE_BASE 0x100000       ; address of kernel in protected mode

%define IMAGE_RMODE_BASE 0x3000         ; address of kernel in real mode

ImageName   db "KERNEL  SYS"            ; kernel file name in 11 bytes

ImageSize db 0

%endif
