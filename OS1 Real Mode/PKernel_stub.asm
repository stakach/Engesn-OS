start:

;****************************************************************************
; Sets up cpu for kernel execution and displays msg.
;****************************************************************************
        mov     ax,cs
        mov     ds,ax
        mov     es,ax


; set 80x50 text mode and 8x8 font
	mov ax,1112h
	xor bl,bl
	int 10h

 	mov ax,0x0100           ;Set big cursor
        mov ch,0
        mov cl,0x0f
        int 0x10

;****************************************************************************
; k_main
; 
; This is the main function of the PK Kernel, Just as in normal c programs.
;
;****************************************************************************
[extern _k_main]		; this is in the c file
  	jmp _k_main



