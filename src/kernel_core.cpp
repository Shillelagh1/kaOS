#include "kernel_console.h"

void memory_test(void){
	__asm__ volatile(
		"push %eax\n\t"
		"mov $0xE801, %ax\n\t"
		"int $0x15\n\t"
		"pop %eax\n\t"
	);
}

extern "C" void hl_kernel_postgdt(void){
	console_initialize();
	console_printline("Hello, World!\nTest.");
	memory_test();
}
