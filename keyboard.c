#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "vga_text.h"
#include "asm_help.h"

struct keyboard_state {
	bool capslock;
	bool numlock;
	bool scrolllock;
	bool lshift_held;
	bool lctrl_held;
	bool lalt_held;
	bool ralt_held;
	bool rctrl_held;
	bool rshift_held;
	int extension_state;
} keyboard_state = {false, false, false, false, false, false, false, false, false, 0};

uint8_t keycodes_1[] = {
//  0     1     2     3     4     5     6     7
//  8     9     A     B     C     D     E     F
	0xff, 0x00, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,		// 0
	0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,		// 10
	0x29, 0x2a, 0x2b, 0x2c, 0x3c, 0x50, 0x31, 0x32,
	0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a,		// 20
	0x3b, 0x10, 0x40, 0x2d, 0x41, 0x42, 0x43, 0x44,
	0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x71,		// 30
	0x53, 0x54, 0x30, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09, 0x0a, 0x61, 0x7f, 0x62,		// 40
	0x68, 0x72, 0x77, 0x63, 0x69, 0x73, 0x78, 0x64,
	0x6a, 0x74, 0x65, 0x75, 0xff, 0xff, 0xff, 0x0b,		// 50
	0x0c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

char keycode_chars[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'`','1','2','3','4','5','6','7','8','9','0','-','=',0,0,0,
	'\t','q','w','e','r','t','y','u','i','o','p','[',']','\\',0,0,
	0,'a','s','d','f','g','h','j','k','l',';','\'','\n',0,0,0,
	0,'z','x','c','v','b','n','m',',','.','/',0,0,0,0,0,
	0,0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,
	0,0,'7','4','1','0',0,'/','8','5','2',0,0,0,0,0,
	0,'8','9','6','3','.',0,'-','+','\n',0,0,0,0,0,0
};

char keycode_chars_shift[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'~','!','@','#','$','%','^','&','*','(',')','_','+',0,0,0,
	'\t','Q','W','E','R','T','Y','U','I','O','P','{','}','|',0,0,
	0,'A','S','D','F','G','H','J','K','L',':','"','\n',0,0,0,
	0,'Z','X','C','V','B','N','M','<','>','?',0,0,0,0,0,
	0,0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0
};

void keyboard_event(uint8_t keycode, bool ispress) {
	switch (keycode) {
		case 0x30:
			if (ispress) {
				keyboard_state.capslock = !keyboard_state.capslock;
			}
			break;
		case 0x61:
			if (ispress) {
				keyboard_state.numlock = !keyboard_state.numlock;
			}
			break;
		case 0x40:
			keyboard_state.lshift_held = ispress;
			break;
		case 0x50:
			keyboard_state.lctrl_held = ispress;
			break;
		case 0x53:
			keyboard_state.lalt_held = ispress;
			break;
		case 0x55:
			keyboard_state.ralt_held = ispress;
			break;
		case 0x57:
			keyboard_state.rctrl_held = ispress;
			break;
		case 0x4b:
			keyboard_state.rshift_held = ispress;
			break;
	}

	if (!ispress) return;
	char charcode = keycode_chars[keycode];
	if (charcode == 0) return;
	if (keycode < 0x60) {
		if (keyboard_state.lshift_held || keyboard_state.rshift_held) {
			charcode = keycode_chars_shift[keycode];
		}
	}
	vga_text_put(&charcode, 1);
}

void irq_keyboard_handler() {
	uint8_t keycode, scancode = inb(0x60);
	bool release;

	switch (keyboard_state.extension_state) {
		case 0:
			if (scancode == 0xE0) {
				keyboard_state.extension_state = 1;
				return;
			} else if (scancode == 0xE1) {
				keyboard_state.extension_state = 2;
				return;
			}

			release = scancode >= 0x80;
			scancode &= 0x7F;
			if (scancode >= 0x60) {
				return;		// UNKNOWN
			}
			keycode = keycodes_1[scancode];
			if (keycode == 0xff) {
				return;		// UNKNOWN
			}
			keyboard_event(keycode, !release);
			return;

		case 1:		// Extension code basics
			keyboard_state.extension_state = 0;

			release = scancode >= 0x80;
			scancode &= 0x7F;
			switch (scancode) {
				case 0x1C:
					keyboard_event(0x79, release);
					return;
				case 0x1D:
					keyboard_event(0x57, release);
					return;
				case 0x35:
					keyboard_event(0x67, release);
					return;
				case 0x38:
					keyboard_event(0x55, release);
					return;
				case 0x47:
					keyboard_event(0x60, release);
					return;
				case 0x49:
					keyboard_event(0x70, release);
					return;
				case 0x4F:
					keyboard_event(0x66, release);
					return;
				case 0x51:
					keyboard_event(0x76, release);
					return;
				case 0x52:
					keyboard_event(0x0d, release);
					return;
				case 0x53:
					keyboard_event(0x0e, release);
					return;
				case 0x5B:
					keyboard_event(0x52, release);
					return;
				case 0x5D:
					keyboard_event(0x56, release);
					return;

				case 0x48:
					keyboard_event(0x58, release);
					return;
				case 0x50:
					keyboard_event(0x59, release);
					return;
				case 0x4B:
					keyboard_event(0x5a, release);
					return;
				case 0x4D:
					keyboard_event(0x5b, release);
					return;
			}
			return;

		case 2:
			keyboard_state.extension_state = 3;
			return;
		case 3:
			keyboard_state.extension_state = 0;
			return;
	}
	return;
}

