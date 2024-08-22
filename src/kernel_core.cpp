#include "kernel_console.h"
#include "except.h"

#define KERNEL_VERSION "v0.0.3"
#define KERNEL_INTRO_MSG ("==== kaOS kernel (" KERNEL_VERSION ") ====")

extern "C" void hl_kernel_postgdt(void){
	console_initialize();
	
	except_initialize();			// Interrupts & PICs setup
	
	console_printline(KERNEL_INTRO_MSG);
	console_printline("\n");		// Introduction message
}
