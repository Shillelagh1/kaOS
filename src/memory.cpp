#include <stdint.h>
#include "kernel_console.h"

uint32_t resp;

extern "C" void _loadPageDir(uint32_t);

typedef struct{
    uint32_t entries[1024];
} page_table __attribute__((aligned(4096)));

uint32_t page_dir[1024] __attribute__((aligned(4096)));
page_table page_tables[1024] __attribute__((aligned(4096)));

extern "C" void _mTest(uint32_t bufaddr);

// Returns the base address of the new page.
// Creates a new 1:1 page and assigns it into the directory.
extern "C" uint32_t memory_set_1to1(int idx){
    for(int i = 0; i < 1024; i++){
        page_tables[idx].entries[i] = ((i * 0x1000) + (idx * 0x400000)) | 3;
    }

    page_dir[idx] = ((unsigned int)page_tables[0].entries) | 3;
    return (idx * 0x400000);
}

extern "C" void memory_init(){
    // Fill the page directory with blank directories
    for (int i = 0; i < 1024; i++){
        page_dir[i] = 0x2;
    }

    // Pages 0-4 are 1:1.
    // Pages 0-3 are kernel program and data pages.
    // Page 4 is a special data page. BIOS functions should perform operations on data here.

    memory_set_1to1(0);
    memory_set_1to1(1);
    memory_set_1to1(2);
    memory_set_1to1(3);
    resp = memory_set_1to1(4);

    _loadPageDir((uint32_t)page_dir);

    // TODO: Memory Test
}