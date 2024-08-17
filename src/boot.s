; Built on the Bare Bones boot file template from 'wiki.osdev.org'
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBFLAGS  equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)


section .multiboot
	align 4
		dd MAGIC
		dd MBFLAGS
		dd CHECKSUM

section .data
	GDT:
		dd 0x00000000, 0x00000000 ; NULL
		dd 0x0000ffff, 0x00df9f00 ; EXEC 0
		dd 0x0000ffff, 0x00df9300 ; DATA 0
		GDT_SIZE: equ $ - GDT
	GDTR:
		dw (GDT_SIZE - 1)
		dq GDT

section .bss
	align 16
	stack_bottom:
	resb 16384 ; 16 KiB
	stack_top:

section .text
	global _start
	_start:
		; Load GDTR
		lgdt [GDTR]

		; Apply GDTR
		jmp 0x08:.CS
	.CS
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		
		; Call high level kernel
		extern hl_kernel_postgdt
		call hl_kernel_postgdt

		; Don't fall out
		xchg bx, bx
	exit:	cli
	.l:	hlt
		jmp .l
