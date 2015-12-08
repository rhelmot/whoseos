// Obviously copied from http://wiki.osdev.org/Bare_Bones

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

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

size_t strlen (const char * str) {
	int result = 0;
	while (str[result]) {
		result++;
	}
	return result;
}

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

void vga_text_putc(char c) {
	vga_text_state.buffer[VGA_TEXT_CURINDEX] = c | VGA_TEXT_COLORHI;
	vga_text_state.current_x++;
	if (vga_text_state.current_x >= VGA_TEXT_SCREEN_WIDTH) {
		vga_text_state.current_x = 0;
		vga_text_state.current_y++;
		if (vga_text_state.current_y >= VGA_TEXT_SCREEN_WIDTH) {
			vga_text_state.current_y = 0;
			// TODO: Scrolling
		}
	}
}

void vga_text_put(char *s, size_t len) {
	for (size_t i = 0; i < len; i++) { 
		vga_text_putc(s[i]);
	}
}

void vga_text_print(char *s) {
	vga_text_put(s, strlen(s));
}

#if defined(__cplusplus)
extern "C"	// remind me why we need this
#endif
void kernel_main() {
	vga_text_init();
	vga_text_print("Hello, world!\n");
}
