#include <stddef.h>
#include <stdint.h>

#include "lib.h"

/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

#define VGA_TEXT_BUFFER 0xB8000
#define VGA_TEXT_SCREEN_WIDTH 80
#define VGA_TEXT_SCREEN_HEIGHT 25

struct vga_text_state {
	size_t current_x;
	size_t current_y;
	uint8_t current_color;
	uint16_t * buffer;
} vga_text_state;

#define VGA_TEXT_COLORHI ((uint16_t) vga_text_state.current_color << 8)
#define VGA_TEXT_CURINDEX (vga_text_state.current_y * VGA_TEXT_SCREEN_WIDTH + vga_text_state.current_x)

void vga_text_init() {
	vga_text_state.current_x = 0;
	vga_text_state.current_y = 0;
	vga_text_state.current_color = (COLOR_BLACK << 4) | COLOR_WHITE;
	vga_text_state.buffer = (uint16_t *) VGA_TEXT_BUFFER;

	for (int i = 0; i < VGA_TEXT_SCREEN_WIDTH * VGA_TEXT_SCREEN_HEIGHT; i++) {
		vga_text_state.buffer[i] = ' ' | VGA_TEXT_COLORHI;
	}
}

void vga_text_newline() {
	vga_text_state.current_x = 0;
	if (++vga_text_state.current_y >= VGA_TEXT_SCREEN_HEIGHT) {
		vga_text_state.current_y = VGA_TEXT_SCREEN_HEIGHT - 1;
		memcpy(vga_text_state.buffer, vga_text_state.buffer + VGA_TEXT_SCREEN_WIDTH, sizeof(uint16_t)*VGA_TEXT_SCREEN_WIDTH*(VGA_TEXT_SCREEN_HEIGHT-1));
		for (int i = VGA_TEXT_SCREEN_WIDTH*(VGA_TEXT_SCREEN_HEIGHT-1); i < VGA_TEXT_SCREEN_WIDTH*VGA_TEXT_SCREEN_HEIGHT; i++) {
			vga_text_state.buffer[i] = ' ';		// force black
		}
	}
}

void vga_text_putc(char c) {
	vga_text_state.buffer[VGA_TEXT_CURINDEX] = c | VGA_TEXT_COLORHI;
	vga_text_state.current_x++;
	if (vga_text_state.current_x >= VGA_TEXT_SCREEN_WIDTH) {
		vga_text_newline();
	}
}

void vga_text_put(const char *s, size_t len) {
	for (size_t i = 0; i < len; i++) { 
		switch (s[i]) {
			case '\n':
				vga_text_newline();
				break;

			default:
				vga_text_putc(s[i]);
				break;
		}
	}
}

void vga_text_print(const char *s) {
	vga_text_put(s, strlen(s));
}
