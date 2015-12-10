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
	vga_text_print("Hello, world!\n");
	kprintf("Lol, number %x\n:)\n", 0);
	kprintf("Lol, number %x\n:)\n", 1);
	kprintf("Lol, number %x\n:)\n", 2);
	kprintf("Lol, number %x\n:)\n", 3);
	kprintf("Lol, number %x\n:)\n", 4);
	kprintf("Lol, number %x\n:)\n", 5);
	kprintf("Lol, number %x\n:)\n", 6);
	kprintf("Lol, number %x\n:)\n", 7);
	kprintf("Lol, number %x\n:)\n", 8);
	kprintf("Lol, number %x\n:)\n", 9);
	kprintf("Lol, number %x\n:)\n", 10);
	kprintf("Lol, number %x\n:)\n", 11);
	kprintf("Lol, number %x\n:)\n", 12);
	kprintf("Lol, number %x\n:)\n", 13);
	kprintf("Lol, number %x\n:)\n", 14);
	kprintf("Lol, number %x\n:)\n", 15);
	kprintf("Lol, number %x\n:)\n", 16);
	kprintf("Lol, number %x\n:)\n", 17);
	kprintf("Lol, number %x\n:)\n", 18);
	kprintf("Lol, number %x\n:)\n", 19);
	kprintf("Lol, number %x\n:)\n", 20);
	kprintf("Lol, number %x\n:)\n", 21);
	kprintf("Lol, number %x\n:)\n", 22);
	kprintf("Lol, number %x\n:)\n", 23);
	kprintf("Lol, number %x\n:)\n", 24);
	kprintf("Lol, number %x\n:)\n", 25);
	kprintf("Lol, number %x\n:)\n", 26);
	kprintf("Lol, number %x\n:)\n", 27);
	kprintf("Lol, number %x\n:)\n", 28);
	kprintf("Lol, number %x\n:)\n", 29);
	kprintf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!!\n", 0);

}
