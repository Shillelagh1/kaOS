%macro isr_err_stub 1
isr_stub_%+%1:
	pushad			; Preserve general purpose registers
	mov eax, [esp+32]	; Get the error code off the stack
	
	push eax
	push %1
    	call EXCEPTION_ERR
    	pop eax
    	pop eax			; Call Handler
    	
    	popad
    	pop eax
    	iret			; Cleanup & Return
%endmacro
%macro isr_no_err_stub 1
isr_stub_%+%1:
	pushad			; Preserve general purpose registers

	push %1
    	call EXCEPTION_NOERR
    	pop eax			; Call Handler

    	popad
    	iret
%endmacro

extern EXCEPTION_ERR
extern EXCEPTION_NOERR
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    	dd isr_stub_%+i
%assign i i+1 
%endrep

; Load the IDTR
global _lodidt
_lodidt:
	mov eax, [esp + 4]
	lidt [eax]
	ret
	
; Initialize the PICs
global _picinit
_picinit:
	
	mov al, 0x11
	out 0x20, al
	out 0xA0, al	; Make PICs listen
	
	mov al, 0x20
	out 0x21, al
	mov al, 0x28
	out 0xA1, al	; Set offset
	
	mov al, 4
	out 0x21, al
	mov al, 2
	out 0xA1, al	; Configure for slavery
	
	mov al, 1
	out 0xA1, al
	out 0x21, al	; Configure to 8086
	
	mov al, 0xFF
	out 0x21, al
	out 0xA1, al
	
	ret
