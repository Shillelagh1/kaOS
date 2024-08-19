uint32_t page_dir[1024] __attribute__((aligned(4096));

extern "C" void memory_pages_init(){
	// Clean and initialize
	for (int i = 0; i < 1024; i++){
		page_dir[i] = 0x00000002
	}
}
