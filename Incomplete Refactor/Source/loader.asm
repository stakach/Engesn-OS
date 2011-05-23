;++
;
; This code is open domain.
;
; Module Name:
;
;	kstub.asm
;
; Abstract:
;
;	Assembley code necessary to control the kernel startup
;
; Author:
;
;	Stephen von Takach 09-March-2004
;
; Revision History:
;
;--

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1
MULTIBOOT_AOUT_KLUDGE	equ 1<<16
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM		equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[BITS 32] 			; protected mode.

[global start]		; Kernel entry point symbol.

[extern _main] 		; this is our C++ code.

[extern __main] 	; this is our C support code.
[extern __atexit]

start:

mov	[_mbinfo], ebx		; Save any multiboot data
mov	[_mbmagic], eax

call __main			; Call our constructors
call _main 			; Call our C++ code
call __atexit		; Call our deconstructors

abc123:			;Hang code in case main returns...

cli 			; interrupts will disturb the halt
hlt 			; halt the CPU
jmp abc123		; In case halt is disturbed


;-------------------------------------------------------------
;	Multi-Boot header.
;-------------------------------------------------------------

[global _mboot]
[global _mbinfo]
[global _mbmagic]
[extern code]
[extern bss]
[extern end]

ALIGN 4
_mboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
	
	dd _mboot		; Location of this descriptor
	dd code			; Start of kernel '.text' (code) section.
	dd bss			; End of kernel '.data' section.
	dd end			; End of kernel.
	dd start		; Kernel entry point (initial EIP).
_mbinfo:
	dd 0
_mbmagic:
	dd 0


