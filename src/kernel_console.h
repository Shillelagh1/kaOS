#pragma once
#include <stdint.h>

extern "C" void console_printline(char* string, int col = -1, int row = -1, uint8_t color=0x0F);
extern "C" void console_printint(int val, int base, uint8_t col=0x0f);
extern "C" void console_putchar(char character, int col, int row, uint8_t color=0x0F);
extern "C" void console_initialize();
extern "C" void console_putcursor(int col, int row);
