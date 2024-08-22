// the gods have stopped me from just adding this to the bootstrap file next to the GDT.
// Most of this is stolen from the OSdev wiki
#include <stdint.h>
#include "libc.h"
#include "kernel_console.h"

#define PIC1		0x20
#define PIC2		0xA0
#define PIC1_DATA 	(PIC1+1)
#define PIC2_DATA	(PIC2+1)

#define IDT_MAX_VECTORS 256

typedef struct {
	uint16_t isr_0;
	uint16_t CS;
	uint8_t zero;
	uint8_t attributes;
	uint16_t isr_1;
}
__attribute__((packed)) interrupt_descriptor_t;

__attribute__((aligned(0x10)))
static interrupt_descriptor_t idt[IDT_MAX_VECTORS];

// Display names for the first 32 interrupts
char* errorDescriptions[]{
	"div",	// 0, division
	"dbg",	// 1, debug
	"nmi",	// 2, non masking interrupt
	"bp",	// 3, breakpoint
	"of",	// 4, overflow
	"br",	// 5, bound rage exceeded
	"opc",	// 6, bad opcode
	"fpu",	// 7, device not available
	"df",	// 8, double fault
	"sov",	// 9, segment overrun
	"tss",	// 10, invalid TSS
	"cso",	// 11, coprocessor segment missing
	"ss",   // 12, stack-segment fault
	"gp",	// 13, general protection fault
	"pf",	// 14, page fault
	"Fh",	// 15, reserved
	"mf",	// 16, floating point
	"ac",	// 17, alignment check
	"mc",	// 18, machine check
	"xm",	// 19, SIMD floating point
	"ve",	// 20, virtualization exception
	"cp",	// 21, control protection exception
	"16h",	// 22, reserved
	"17h",	// 23, reserved
	"18h",	// 24, reserved
	"19h",	// 25, reserved
	"1Ah",	// 26, reserved
	"1Bh",	// 27, reserved
	"hv",	// 28, hypervisor
	"vc",	// 29, VMM communication exception
	"sx",	// 30, Security exception
	"1Fh",	// 31, reserved
};

struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr;

// Thanks again OSdev wiki, I'm too lazy for this method.
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    interrupt_descriptor_t* descriptor = &idt[vector];

    descriptor->isr_0        	= (uint32_t)isr & 0xFFFF;
    descriptor->CS      	= 0x08;
    descriptor->attributes     	= flags;
    descriptor->isr_1	  	= (uint32_t)isr >> 16;
    descriptor->zero       	= 0;
}

inline void wait(){
	outb(0x80, 0);
}

static bool vectors[IDT_MAX_VECTORS]; 

extern "C" void* isr_stub_table[];
extern "C" void _lodidt(uint32_t idt);
extern "C" void _picinit();
extern "C" void except_initialize(){	
	// Disable interrupts
	asm("cli");

	// Generate IDTR
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(interrupt_descriptor_t) * (IDT_MAX_VECTORS - 1);
	
	for (uint8_t i = 0; i < 32; i++) { 
		idt_set_descriptor(i, isr_stub_table[i], 0x8E);
		vectors[i] = true;
	}
	
	// Load the IDT
	_lodidt((uint32_t)&idtr);
	
	// Initialize PICs
	_picinit();
	
	// Re-enable interrupts
	asm("sti");
}

// Common exception message handler
void EXCEPTION_COMMON(uint32_t val){
	console_printline("## Exception \"");
	console_printline(errorDescriptions[val]);
	console_printline("\" (0x");
	console_printint(val, 16);
	console_printline(")\n");
}

// Exception w/ error code handler
extern "C" void EXCEPTION_ERR(uint32_t val, uint32_t err){
	EXCEPTION_COMMON(val);

	console_printline("## Error: ");
	console_printint(err, 10);
	console_printline(" (0x");
	console_printint(err, 16);
	console_printline(")\n");
	
	// Special info print cases
	switch (val){
		// GP Fault
		case 13:
			if ((err & 1) > 0){
				console_printline("## External\n");
			}
			else{
				console_printline("## Internal\n");
			}
			
			switch ((err >> 1) & 3){
				case 0:
					console_printline("## From GDT\n");
					break;
				case 2:
					console_printline("## From LDT\n");
					break;
				case 1:
				case 3:
					console_printline("## From IDT\n");
					break;
			}
			
			console_printline("## Index: 0x");
			console_printint((val >> 3) & 0x1FFF, 8);
			console_printline("\n");
			
			if (err >> 16 > 0){
				console_printline("## ! Use of reserved err space (0x");
				console_printint(err >> 16, 8);
				console_printline(")\n");
			}
		default:
		break;
	}
	
	asm("cli");
	asm("hlt");
}

// Exception w/o error code handler
extern "C" void EXCEPTION_NOERR(uint32_t val){
	EXCEPTION_COMMON(val);
	
	asm("cli");
	asm("hlt");
}
