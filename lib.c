#include <stdint.h>
#include <stddef.h>

#include "vga_text.h"

size_t strlen (const char * str) {
	int result = 0;
	while (str[result]) {
		result++;
	}
	return result;
}

void * memcpy (void * dest, const void * src, size_t count) {
	char * dest_c = dest;
    const char * src_c = src;
	for (size_t i = 0; i < count; i++) {
		dest_c[i] = src_c[i];
	}
	return dest;
}

void * memset (void * dest, int c, size_t count) {
	char * dest_c = dest;
	for (size_t i = 0; i < count; i++) {
		dest_c[i] = c;
	}
	return dest;
}

void kprintf(const char * fmt, int x) {
	const char * fp = fmt;
	while (*fp) {
		if (*fp == '%') {
			char c;
			fp++;
			if (*fp == 'x') {
				c = x >> 4;
				if (c > 9) c += 0x41 - 0xa;
				else c += 0x30;
				vga_text_put(&c, 1);

				c = x & 0xf;
				if (c > 9) c += 0x41 - 0xa;
				else c += 0x30;
				vga_text_put(&c, 1);
			}
		} else {
			vga_text_put(fp, 1);
		}
		fp++;
	}
}
