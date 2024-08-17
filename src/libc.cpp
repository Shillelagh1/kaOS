#include <stdint.h>

extern "C" void outb(uint16_t port, uint8_t val){
	asm volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

extern "C" uint8_t inb(uint16_t port){
	uint8_t ret;
	asm volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
}
