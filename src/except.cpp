// the gods have stopped me from just adding this to the bootstrap file next to the GDT.
// Most of this is stolen from the OSdev wiki
#include <stdint.h>
#include "kernel_console.h"

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

static bool vectors[IDT_MAX_VECTORS]; 
extern "C" void* isr_stub_table[];
extern "C" void _lodidt(uint32_t idt);
extern "C" void except_initialize(){
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(interrupt_descriptor_t) * (IDT_MAX_VECTORS - 1);
	
	for (uint8_t i = 0; i < 32; i++) {
		idt_set_descriptor(i, isr_stub_table[i], 0x8E);
		vectors[i] = true;
	}
	
	_lodidt((uint32_t)&idtr);
}

extern "C" void EXCEPTION_DEFAULT(){
	console_printline("== Fatal Exception (general)");
}
