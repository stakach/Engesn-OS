
[global _GET_A20_STATE]
[global _A20_ENABLE]
[global _A20_DISABLE]

;
; Determins the state of the line A20 by reading the Ouput Port
; register of the KBC.
; If an error occured, the zero flag will be set.
; Else, it returns the in AH the value of the Output port on the KBC.
; Bit 1 of AH reflects the state,
;	0 - A20 disabled
;	1 - A20 enabled
; To check you would do something like
;	test ah, 2	; 2 = 0010b
;	jz A20_DISABLED
;


;****************************************************************************
; Returns the status of the A20 Gate:
;****************************************************************************

_GET_A20_STATE:
		call	A20Write		; Wait till the input register is empty
                jz      exit+1                 ; Bypas the reset int's
		cli				; Disable ints so we'll get our value

		mov	al, 0D0h		; Send command to the 8042 command register
		out	64h, al			; to tell it we want to Read the Output Port
		call	A20Read			; Wait till the 8042 output register
                jz      exit                   ; has something in it's buffer
		
		in	al, 60h			; Get it
		mov	ah, al
		call	A20Write		; Make sure the input register is empty
                jz      exit

		mov	al, 0FFh		; KBC NOP command, does nothing
		out	64h, al
		call	A20Write

exit:           sti
		ret



;****************************************************************************
; Sets the A20 gate for access:
;****************************************************************************

_A20_ENABLE:
		call	A20Write
                jz      exit2

		cli				; Only need for reading value from KBC
		mov	al, 0D0h
		out	64h, al
		call	A20Read
                jz      enable_int

		in	al, 60h			; Get current status
		mov	ah, al
		call	A20Write
enable_int:    sti
                jz      exit2

		mov	al, 0D1h		; Tell the KBC we want to write to the
		out	64h, al			; Out Put Port next
		call	A20Write
                jz      exit

		mov	al, ah
		or	al, 2			; Enable line A20
		out	60h, al
		call	A20Write
                jz      exit

		mov	al, 0FFh		; NOP
		out	64h, al
		call	A20Write

exit2:          call chka20
		ret

		

;****************************************************************************
; Disables A20 access:
;****************************************************************************

_A20_DISABLE:
		call	A20Write
                jz      exit3

		cli				; Only need for reading value from KBC
		mov	al, 0D0h
		out	64h, al
		call	A20Read
                jz      enable_int2

		in	al, 60h
		mov	ah, al
		call	A20Write
enable_int2:    sti
                jz      exit3

		mov	al, 0D1h
		out	64h, al
		call	A20Write
                jz      exit3

		mov	al, ah
		and	al, 0FDh		; Disable line A20
		out	60h, al
		call	A20Write
                jz      exit3

		mov	al, 0FFh
		out	64h, al
		call	A20Write

exit3:          call chka20
		ret





; Waits until the Input register is empty or until a short amount
; of time has elapsed. It waits till bit 1 of the KBC Read Status
; Port is clear, Zero.
; Sets the Zero flag if an error occured.
A20Write:	xor	cx, cx			; Reasonable wait
  get:          in      al, 64h                 ; Get KBC read status
		test	al, 02			; See if buffer empty, bit 1 clear
                jz      clear
		dec	cx
                jz      exit4
                jmp     SHORT get
 clear:        inc     al                      ; Clear ZR
 exit4:         ret

; Waits until the Read Status Port, 64h, contains data by checking the state
; of bit 0. It waits till it is set or too much time has gone by.
; Sets the Zero flag if an error occured,
A20Read:	xor	cx, cx
 get2:          in      al, 64h
		test	al, 01			; If the 'output buffer' is full, has
                jnz     exit5                   ; something for me
		dec	cx
                jnz     get2
 exit5:         ret



;****************************************************************************
; Test A20 gate access. Called by _A20_ENABLE: (Not for external access)
;****************************************************************************

EXTERN _A20E
chka20:
    push ax                 ; (this is real mode code)
    push ds
    push es
        xor ax,ax
        mov ds,ax           ; DS=0
        dec ax
        mov es,ax           ; ES=FFFFh
        mov ax,[es:10h]     ; read word at FFFF:0010 (1 meg)
        not ax              ; 1's complement
        push word [0]       ; save word at 0000:0000 (0)
            mov [0],ax      ; word at 0 = ~(word at 1 meg)
            mov ax,[0]      ; read it back
            cmp ax,[es:10h] ; fail if word at 0 == word at 1 meg
        pop word [0]
    pop es
    pop ds
    pop ax
    je a20_not_enabled
    mov eax, 1
    mov [_A20E], eax
    ret

a20_not_enabled:
	mov eax, 0
	mov [_A20E], eax
	ret
