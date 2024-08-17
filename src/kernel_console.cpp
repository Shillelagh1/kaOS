#include <stdint.h>
#include <stdbool.h>
#include "libc.h"


#define CONSOLE_W 80
#define CONSOLE_H 25

int cursor_col;
int cursor_row;
uint16_t* console_buf;

extern "C" void console_putcursor(int col, int row){
	uint16_t idx = col + row * CONSOLE_W;
	
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (idx & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((idx >> 8) & 0xFF));
}

extern "C" void console_initialize(){
	console_buf = (uint16_t*) 0xB8000;
	cursor_col = 0;
	cursor_row = 0;
}

extern "C" void console_putchar(char ch, int col, int row, uint8_t color){
	if (col < 0) col = cursor_col;
	if (row < 0) row = cursor_row;

	uint16_t to_place = ch;
	to_place |= color << 8;
	console_buf[col + row * CONSOLE_W] = to_place;
}

extern "C" void console_printline(char* string, int col, int row, uint8_t color){
	bool update_cursor = false;
	if (col < 0 || row < 0){
		col = cursor_col;
		row = cursor_row;
		update_cursor = true;
	} 
	
	for (int i = 0; string[i]; i++){
		switch (string[i]){
			case '\n':
				col = 0;
				row ++;
				break;
			default:
				console_putchar(string[i], col, row, color);
				col++;
				if (col >= CONSOLE_W){
					col = 0;
					row ++;
				}
		}
	}
	
	if(update_cursor){
		cursor_col = col;
		cursor_row = row;
		console_putcursor(cursor_col, cursor_row);
	}
}
