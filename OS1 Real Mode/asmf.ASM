

;****************************************************************************
; C standard lib outport functions::
;****************************************************************************

global _outportb
_outportb:
        push 	bp
	mov 	bp, 	sp
        mov     dx,     [bp+4]
        mov     al,     [bp+6]
        out     dx,     al
        pop	bp
        ret


global _outportw
_outportw:
        push 	bp
	mov 	bp, 	sp
        mov     dx,     [bp+4]
        mov     ax,     [bp+6]
        out     dx,     ax
        pop	bp
        ret


;****************************************************************************
; C standard lib inport functions::
;****************************************************************************

global _inportb
_inportb:
        push 	bp
	mov 	bp, 	sp
        mov     dx,     [bp+4]
        in      al,     dx
        pop bp
        ret


global _inportw
_inportw:
        push 	bp
	mov 	bp, 	sp
        mov     dx,     [bp+4]
        in      ax,     dx
        pop	bp
        ret

;****************************************************************************
; Disables and enables interupts:
;****************************************************************************
global _disable
_disable:
        cli
	ret

global _enable
_enable:
	sti
	ret

;****************************************************************************
; Halts the cpu:
;****************************************************************************
global _halt
_halt:
	hlt
	ret


;****************************************************************************
; Waits for keyboard input and returns the character
;****************************************************************************

[global _bios_getch]
_bios_getch:
push bp
mov bp, sp
xor ah, ah
int 0x16
and ax, 0xFF
pop bp
ret


[global _bios_getscan]
_bios_getscan:
push bp
mov bp, sp
mov ah, 10h
int 0x16
pop bp
ret


;****************************************************************************
; Waits for a 10000th of a second if a value of one is passed.
;****************************************************************************

[global _MicroDelay]
_MicroDelay:
push bp
mov bp, sp
mov cx, word [bp+5]
mov dx, word [bp+4]
mov ah, 0x86
int 0x15
pop bp
ret



;****************************************************************************
; Functions i'm not using at the moment: (Later implementation)
;****************************************************************************

;global _nullintw
;_nullintw:
;        iret

;global _nullintd
;_nullintd:
;        iretd

;global _getcs 
;_getcs:
;        mov     ax,     cs
;        ret


;global _getds
;_getds:
;        mov     ax,     ds
;        ret


;global _getss
;_getss:
;        mov     ax,     ss
;        ret