section .data
    cr0_restore:
        dd 0

section .text
extern KaOS_GDT
    global enterProtectedMode
    enterProtectedMode:
            push eax

            cli

            lgdt [KaOS_GDT]

            mov eax, cr0
            or eax, 0x80000001
            mov cr0, eax            ; Set CR0.PE & CR0.PG

            jmp 0x08:.CS            ; Far jump back into segment 1 (shouldn't be necessary?)
        .CS:
            mov ax, 0x08
            mov ds, ax
            mov es, ax
            mov fs, ax
            mov gs, ax
            mov ss, ax

            sti

            pop eax
        ret