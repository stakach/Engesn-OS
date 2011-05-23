; Loader.asm

[BITS 32] ; protected mode

[global start] 
[extern _main] 		; this is our C++ code
[extern __main] 	; this is our C support code
[extern __atexit]

start:
call __main
call _main 	; call our C++ code
call __atexit

cli 	; interrupts will disturb the halt
hlt 	; halt the CPU
