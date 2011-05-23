; bootf.asm  FAT12 floppy bootstrap for protected mode image
; Version 2.0, May 1, 1999
; Sample code
; by John S. Fine  johnfine@erols.com
; I do not place any restrictions on your use of this source code
; I do not provide any warranty of the correctness of this source code
;_____________________________________________________________________________
;
; Documentation:
;
;   1)  Most of the documentation is in comments at the END of this file.
;   2)  Some is in the read.me file that came with this file.
;   3)  In the comments within the source code are several uses of {}, each
;       of these indicates a section of documentation at the end of this file
;       which applies to that part of the source code.
;   4)  The major way I reduced code size was to rely on left-over values in
;       registers and/or to initialize registers at points where doing so was
;       cheap.  To make that understandable, there are several lines beginning
;       with either ";>" or ";>>".  These lines describe the register contents
;       at that point in the code.  ";>" indicates that the value is required
;       by the next section of code.  ";>>" indicates that the value passes
;       through the next section unmodified and is needed later.
;    5) I made this code by modifying my bootp example.  If something doesn't
;       make sense here, maybe comparison to the other version will help.
;_____________________________________________________________________________
;
; Limitations:
;
;    For simplicity, I made several assumptions about the environment.  Some
; of those may need to be expanded in a future version.  Except for the first,
; they are all things that could be checked by a "SYS" program that installs
; this bootstrap.  In general it makes more sense (where possible) for a SYS
; program to check compatibility issues than for a bootstrap to check them
; itself.
;
;    If these limits are violated, this code simply bombs.  It does not check
; the limits nor display error messages.
;
; 1) Runs only on a 386+
; 2) The FAT structure must be the first thing on the drive (no partitions).
;    It just uses "reserved" sectors before the FAT, not "hidden" etc.
; 3) The sector size must be 512 bytes
; 4) There are less than 256 sectors per fat (safe, since FAT12 has less than
;    4096*1.5 bytes per FAT).
; 5) LBN of root directory less than 65536 (safe in unpartitioned FAT12)
;_____________________________________________________________________________

	%include "gdt.inc"

	struc	BB		;FAT Boot block
		resb	0xD	;Things we ignore
BB_clu		resb	1	;Sectors per cluster
BB_res		resw	1	;Reserved sectors
BB_fats		resb	1	;Number of FATs
BB_root		resw	1	;Root directory entries
		resb	3	;Things we ignore
BB_fat		resw	1	;Sectors per fat
BB_sec		resw	1	;Sectors per track
BB_head		resw	1	;Heads
	endstruc

SEGMENT	START USE16

;>   cs = 0
;>>  dl = drive we were booted from

boot:	jmp short start			;Standard start of boot sector
	nop
	resb	0x3B			;Skip over parameters (set by format)

start:	

; set 80x50 text mode and 8x8 font
	mov ax,1112h
	xor bl,bl
	int 10h

 	mov ax,0x0100           ;Set big cursor
        mov ch,0
        mov cl,0x0f
        int 0x10


	cli				;{0}
	lgdt	[cs:gdt]		;Load GDT
	mov	ecx, CR0		;Switch to protected mode
	inc	cx
	mov	CR0, ecx		;{5}
.5:	in	al, 0x64		;Enable A20 {4A}
	test	al, 2
	jnz	.5
	mov	al, 0xD1
	out	0x64, al
.6:	in	al, 0x64
	and	ax, byte 2
	jnz	.6
	mov	al, 0xDF
	out	0x60, al

;>   ah = 0
;>   dl = drive we were booted from

	mov	al, flat_data		;Selector for 4Gb data seg
	mov	ds, ax			;{2} Extend limit for ds
	mov	es, ax			;Extend limit for es
	dec	cx			;Switch back to real mode
	mov	CR0, ecx		;{5}

	mov	[boot], dl		;Save drive number we came from
	mov	sp, 0x800		;{1B}

	xor	eax, eax		;Segment
	mov	ds, ax			;
	mov	ss, ax
	mov	es, sp			;Read directory at 800:0 {1C}

;>   eax = 00000000

	mov	al, [boot+BB_fats]	;Number of FATs
	mul	byte [boot+BB_fat]	;Times size of FAT
	add	ax, [boot+BB_res]	;Plus Sectors before first FAT
					;eax = LBN of Root directory
	movzx	edi,word [boot+BB_root]	;Root directory entries
	push	di			; used again later
	dec	di			;Convert to number of sectors
	shr	di, 4			;  16 directory entries per sector
	inc	di
	call	read_sectors

;>  eax  = LBN of root directory
;>  edi  = length of root directory in sectors
;>  [sp] = length of root directory in entries
;>  esi  = 00000000

	lea	ebp, [eax+edi]		;ebp = LBN of cluster 2

	pop	bx			;Root directory entries
	xor	di, di			;Point at directory {1C}
.20:	mov	si, file_name		;Name of file we want
	xor	ecx, ecx
	mov	cl, 11
	a32 rep cmpsb			;Found the file?
	je	found			;Yes
	add	cl, 21			;Offset to next directory entry
	add	edi, ecx		;Advance to next entry
	dec	bx			;Loop through all entries
	jnz	.20

	;Couldn't find file in directory
boot_error:
disk_error:	
	mov	ax, 0xE07		;{3}
	int	10h
	jmp short $

;>> ecx   = 00000000
;> es     = 800
;> es:edi = Directory entry of file
;> ebp    = LBN of cluster 2
;> eax    = 0000????

found:	push	word [es:edi+0xF]	;Starting cluster of file
	mov	di, [boot+BB_fat]	;Size of FAT (in sectors)
	mov	ax, [boot+BB_res]	;LBN of FAT
	call	read_sectors

	mov	bx, 0x4000
	mov	es, bx			;es = 0x4000
	mov	edi, 0x100000-0x40000	;{1D}{4B} One megabyte minus ES base
.10:

;>>    ecx = 0000????
;>    [sp] = Next cluster of file
;>     esi = 0000????
;>>    edx = 0000????
;>  es:edi = Destination address
;>     ebp = LBN of cluster 2
;>      ds = 0

	xor	eax, eax
	pop	si			;Next cluster of file
	mov	bx, si
	cmp	si, 0xFF8		;Valid cluster?
	jae	eof			;No: assume end of file
					;Yes: (c-bit set)
	rcr	bx, 1			;bx = 0x8000 + cluster/2
	mov	bx, [bx+si]		;Get word containing FAT entry
	jnc	.11			;Entry is low 12 bits
	shr	bx, 4			;Entry was high 12 bits
.11:	and	bh, 0xF			;Mask to just 12 bits
	push	bx			;Save cluster after next
	push	di			;Save destination address {7}
	mov	al, [boot+BB_clu]	;Size of each cluster
	mov	di, ax			;  (in sectors)
	dec	si
	dec	si
	mul	esi			;Times cluster number minus 2
	add	eax, ebp		;Plus LBN of cluster 2	
	call	read_sectors		;Read that cluster

;>     ecx = 0000????
;>>    edx = 0000????
;>      di = Clustersize in sectors
;>     esi = 0
;>>    ebp = LBN of cluster 2
;>    [sp] = Bottom 16-bits of destination address {7}
;>> [sp+2] = Following cluster
;>      ds = 0
;>      es = 4000

	mov	cx, di			;Cluster size in sectors
	xchg	ch, cl			;Cluster size in words
	pop	di			;Restore destination address {7}
	es a32 rep movsw
	jmp short .10			;Loop until end of file

;>     eax = 0
;>     edx = 0000????
;>      bx = 0FF?

eof:
	mov	dx, 0x9C00
	mov	es, dx			;es = 9C00
	xor	di, di			;{1E} Address of page tables WRT es
	mov	dh, 4096/256		;edx = 4096
.10:	mov	cx, 1024
	mov	al, 7
.20:	stosd
	add	eax, edx
	int	8			;{8}
	loop	.20
	shr	eax, 2			;{4C} (first time only) 4Mb / 4 = 1Mb
	neg	bl			;Done just one page?
	jns	.10			;Yes: do one more

	cli				;{6}

	mov	eax, 0x9C007		;First page tbl pointer in page dir
	stosd				;{1H}
	mov	ax, (1024-3)*2
	xchg	ax, cx
	rep stosw
	mov	ax, 0xD007		;0FF800000 page tbl pointer
	stosd				;{1F}
	mov	ah, 0xE0		;Page directory self pointer
	stosd				;{1G}
	mov	al, 0
	mov	CR3, eax		;Set up page directory
	mov	eax, CR0		;Turn on paging and protected mode
	or	eax, 0x80000001
	mov	CR0, eax
	mov	cl, flat_data		;Setup ds and es
	push	cx			;{5}
	pop	ds
	mov	es, cx
	jmp dword 8:0xFF800000		;Go
	
read_sectors:
; Input:
;	EAX = LBN
;	DI  = sector count
;	ES = segment
; Output:
;	EBX high half cleared
;	DL = drive #
;	EDX high half cleared
;	ESI = 0
; Clobbered:
;	BX, CX, DH

	push	eax
	push	di
	push	es

.10:	push	eax		;LBN

	cdq			;edx = 0
	movzx	ebx, byte [boot+BB_sec]
	div	ebx		;EAX=track ;EDX=sector-1
	mov	cx, dx		;CL=sector-1 ;CH=0
	sub	bl, dl		;BX = max transfer before end of track
	cmp	di, bx		;Do we want more than that?
	ja	.20		;Yes, do just this much now
	mov	bx, di		;No, do it all now
.20:	mov	esi, ebx	;Save count for this transfer.

	inc	cx		;CL=Sector number
	xor	dx, dx
	mov	bl, [boot+BB_head]
	div	ebx		;EAX=cylinder ;EDX=head

	mov	dh, dl		;Head
	mov	dl, [boot]	;Drive
	xchg	ch, al		;CH=Low 8 bits of cylinder number; AL=0
	shr	ax, 2		;AL[6:7]=High two bits of cylinder
	or	cl, al		;CX = Cylinder and sector

	mov	ax, si		;Sector count
	mov	ah, 2		;Read
	xor	bx, bx
	push	ax
	int	13h
	pop	ax
	jnc	.30

	int	13h		;If at second you don't succeed, give up
	jc near	disk_error

.30:	pop	eax
	add	eax, esi	;Advance LBN

	push	si
	shl	si, 5
	mov	bx, es
	add	bx, si		;Advance segment
	mov	es, bx
	pop	si

	sub	di, si
	ja	.10

	pop	es
	pop	di
	pop	eax
	xor	si, si
	ret	

file_name db 'KERNEL  BIN'

gdt	start_gdt		;{9}

flat_code	desc	0, 0xFFBFF, D_CODE+D_READ+D_BIG+D_BIG_LIM

flat_data	desc	0, 0xFFFFF, D_DATA+D_WRITE+D_BIG+D_BIG_LIM

	end_gdt

	resb 0x1FE+$$-$
	db	0x55, 0xAA		;Standard end of boot sector
;_____________________________________________________________________________
;
;  Build/Install instructions:
;
;  *)  NASM -f obj bootf.asm
;
;  *)  JLOC bootf.lnk bootf.bin
;
;  *)  Start with a formatted floppy.
;
;  *)  PARTCOPY bootf.bin 0 3 -f0 0
;      This step overwrites the JMP at the start of the floppy with a JMP $+3E
;      With many formatters, that matches the JMP that the formatter put there
;      and makes no difference.  In most other cases it is safe to overwrite
;      the JMP.  If you want to preserve a different JMP, change the first
;      "resb" in bootp.asm to adjust the address of "start".
;
;  *)  PARTCOPY bootf.bin 3E 1C2 -f0 3E
;      This step copies the rest of bootp.bin to the floppy at offset 3E,
;      skipping the parameters set up by format, so the diskette will still be
;      useable as a DOS diskette.
;
;  *)  Copy some protected mode image to the file C:\KERNEL.BIN.
;
;  *)  Boot the floppy.
;_____________________________________________________________________________
;
;   I left out most of the error handling (what do you expect for 1B1 bytes).
; For errors which it does detect, it just beeps once and hangs.  Errors (and
; unsupported conditions) which it doesn't even try to detect include:
;   a)  Not running on a 386 or better
;   b)  Active partition is not a FAT16 partition
;   c)  Root directory is more than 608Kb long.
;   d)  TEST.BIN more than 4Mb long
;   e)  Total RAM less than 1MB plus actual size of TEST.BIN
; Errors which it does check for include:
;   a)  C-bit set after any int 13h
;   b)  No active partition
;   c)  No "TEST.BIN" in the root directory
; 
;    If this were a partition boot, you probably could leave out the code of
; reading the MBR, and finding and reading partition boot.  Then you could
; add a few error checks.  As a test boot used from a floppy, it needs to
; do that extra work and you just shouldn't use it if the basic conditions
; aren't valid.  As part of a large multi-boot (another example I hope to
; document) it could be bigger than one sector.
;_____________________________________________________________________________
;
; {} Documentation connected to specific source code sections:
;
;{0}  I wasn't sure what to assume about the CPU state when the BIOS (or
; earlier boot) transfers control here.  Probably interrupts are disabled and
; DS=0;  But I didn't assume that.  Assuming that could save a couple bytes
; of code.
;
;{1}  Memory use:
;  {A} The boot itself is at 0:7C00
;  {B} The top of the stack is 0:800
;  {C} The directory and FAT are both read in and accessed starting at 800:0
;  {D} The image is read in, then copied to 1MB (physical)
;  {E} The page tables are built at 9C000 physical
;  {F} The image is mapped to FFF80000 linear
;  {G} The page tables are mapped to FFFC0000 linear
;  {H} The first 4Mb is mapped linear = physical
;
;{2}  Most of this code runs in "big real mode".  That allows the code to make
;  int 13h (and other) BIOS calls, but also to access extended memory directly.
;
;{3}  I left out most error checks, and just beep once for the errors that are
; checked.  That makes this code suitable for learning about pmode booting and
; for test use by pmode kernel developers.  To package with an OS as end-user
; boot code, you need to either take out some of this code or load from
; some larger place, and add some real error messages.
;
;{4}  One way you might want to reduce the work of the boot is to leave the job
; of enabling A20 to the kernel startup code:
;  {A}  Don't enable A20 here.
;  {B}  Load the image at 2Mb instead of at 1Mb.  (Only odd Megabytes are
;       unusable when A20 is disabled).
;  {C}  "shr eax,1" to convert 4Mb to 2Mb.
; Note that the current version has a minimum memory requirement of 1Mb plus
; size of the image.  This change would require 2Mb plus the size of the image.
;
;{5}  The first version of this code changed the value of segment registers
; directly after switching to pmode.  It worked on all but one machine that I
; tested it on (a 386).  On that machine, on the second switch to protected
; mode, the first segment register loaded got the right selector but a bad
; descriptor.  I have read that you need a JMP after to switching to pmode, but
; successfully written MANY programs without that JMP and seen many more
; written by others.  I guess there is a case in which a delay is required (the
; usual "flush the prefetch queue" theory doesn't fit the observed facts
; because I fixed the problem by changing "mov ds,cx" to "push cx", "pop ds";
; They each take only two bytes in the prefetch queue.
;   Further testing has shown that on a 386 you may need more than a short
; instruction worth of delay after switching back to real mode.  Any memory or
; I/O read is enough, so I rearranged the code to have at least one such read
; between a change of CR0 bit 0 and a write to an sreg.  The "jmp short $+2"
; that others recommend would work because it forces a true fetch access (on
; a 386 at least).  I don't use it because I don't want to waste two bytes in
; boot code.
; 
;
;{6}  Most of this code doesn't care whether interrupts are enabled or not, so
; I never enable them after the initial cli.  BIOS's correctly enable
; interrupts DURING the processing of int 13h, if they need interrupts.
; Unfortunately, some BIOSs also enable interrupts on exit from int 13h.
; Interrupts must be disable for the switch to pmode, so another cli is
; required.
;
;{7}  Only the low half of edi is used by read_sectors, so the high half
; doesn't need to be saved and restored across that use.
;
;{8}  I call int 8 (IRQ 0) many times.  This is done in case the code was
; loaded from floppy.  It tricks the BIOS into turning the floppy motor off.
; I don't like to start my pmode tests with the floppy motor on.
;
;{9}  The first entry in a GDT is never used by the CPU.  A zero selector is
; defined as being safe to put into a segment register without loading a
; descriptor from the GDT (You can't access memory through it).  I generally
; use the first 6 bytes of the GDT as a self pointer.  Once you are used to
; this convention, it makes code more readable.  The macros in gdt.inc set it
; up.
;_____________________________________________________________________________
