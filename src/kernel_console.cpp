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

extern "C" void console_putchar(char ch, int col=-1, int row=-1, uint8_t color=0x0F){
	if (col < 0) col = cursor_col;
	if (row < 0) row = cursor_row;

	uint16_t to_place = ch;
	to_place |= color << 8;
	console_buf[col + row * CONSOLE_W] = to_place;
}

extern "C" void console_printline(char* string, int col=-1, int row=-1, uint8_t color=0x0F){
	// Determine if we are printing a line in a custom location or at the end of the console stream
	bool update_cursor = false;
	if (col < 0 || row < 0){
		col = cursor_col;
		row = cursor_row;
		update_cursor = true;
	} 
	
	// Print the string
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

extern "C" void console_printint(int val, int base, uint8_t color){	
	if (val == 0){
		console_printline("0");
		return;
	}

	// Print negative sign.
	if (val < 0 && base == 10){
		console_putchar('-');
		cursor_col ++;
		if(cursor_col >= CONSOLE_W){	// Forward Wrap.
			cursor_col = 0;
			cursor_row ++;
		}
	}
	
	// TODO: Come up with a better way? This is very expensive.
	// Determine the number of characters we need to forward step.
	int numchars = 0;
	int tval = val;
	while (tval){
		tval /= base;
		numchars ++;
	}
	
	// Step forward to the end of where the number will be.
	cursor_col += numchars - 1;
	if (cursor_col >= CONSOLE_W){	// Forward Wrap.
		cursor_col = 0;
		cursor_row ++;
	}
	
	int ecol = cursor_col + 1;
	int erow = cursor_row;
	console_putcursor(cursor_col + 1, cursor_row);
	
	// Begin printing the number from right to left.
	char ch;
	while (val){
		// Decide which character to print.
		ch = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + val % base];
		val /= base;
		console_putchar(ch);
		
		// Step backwards.
		cursor_col --;
		if (cursor_col < 0){	// Negative Wrap.
			cursor_row --;
			cursor_col = CONSOLE_W - 1;
		}
	}
	
	cursor_col = ecol;
	cursor_row = erow;
}
