[bits 16]

section .data
    cr0_restore:
        dd 0

section .text
extern KaOS_GDT

    global enterRealMode
    enterRealMode:        
            push eax

            cli

            mov eax, cr0
            mov [cr0_restore], eax
            and eax, 0x7FFFFFFE
            mov cr0, eax            ; Clear CR0.PG & CE0.PE

            sti
        ret