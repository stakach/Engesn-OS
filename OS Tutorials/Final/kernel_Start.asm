; Kernel stub

[BITS 32] 			; protected mode

[global start] 
[extern _main] 		; this is our C++ code
[extern __main] 	; this is our C support code
[extern __atexit]

start:

mov ax, 0x10		; Setup our stack (608kb)
mov ss, ax			;------------------------
mov ebp, 0x9FFFF	; stack base ------------
mov esp, ebp		; stack pointer ---------

call __main			; Call our constructors
call _main 			; Call our C++ code
call __atexit		; Call our deconstructors

abc123:
cli 			; interrupts will disturb the halt
hlt 			; halt the CPU
jmp abc123		; In case halt is disturbed
