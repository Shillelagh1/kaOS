bits 32
section .text

global _loadPageDir
_loadPageDir:
    push eax

    ; Load page directory address into CR3
    mov eax, [esp + 8]
	mov cr3, eax

    ; Enable paging with CR0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    jmp .C
.C:

    pop eax
	ret

global _mTest
_mTest:
    mov edi, [esp + 4]

    xor ebx, ebx
    mov edx, 0x534D4150
    mov eax, 0xE820
    mov ecx, 24
    int 0x14          ; 0xE820 interrupt

    ret