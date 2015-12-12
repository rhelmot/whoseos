// Obviously copied from http://wiki.osdev.org/Bare_Bones

#include <stdbool.h>
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

#include "lib.h"
#include "vga_text.h"

extern void setup_flat_gdt();
extern void setup_interrupts();
void init_stuff() {
	setup_flat_gdt();
	setup_interrupts();
}

void kernel_main() {
	init_stuff();
	vga_text_init();
}
