#include <stdint.h>
#include <stddef.h>

#include "lib.h"
#include "asm_help.h"

typedef struct idt_entry {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
} idt_entry_t;

typedef struct pusha {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
} pusha_t;

idt_entry_t kernel_idt[256];

void (*interrupt_handlers[256])(pusha_t*);

int irq_timer_counter = 0;
void irq_timer_handler(pusha_t * regs) {
	irq_timer_counter++;
	*((char*)0xB80FE) = (irq_timer_counter % 10) + '0';
	*((char*)0xB80FF) = 15;
}

extern void irq_keyboard_handler(pusha_t * regs);

void setup_interrupts() {
	for (int i = 0; i < 256; i++) {
		interrupt_handlers[i] = NULL;
	}
	interrupt_handlers[0x20] = irq_timer_handler;
	interrupt_handlers[0x21] = irq_keyboard_handler;

#define MAKE_INTERRUPT_ENTRY(N)												\
	extern void * interrupt_entry_##N;										\
	kernel_idt[0x##N].offset_1 = ((uint32_t)&interrupt_entry_##N) & 0xffff;		\
	kernel_idt[0x##N].selector = 0x8;											\
	kernel_idt[0x##N].zero = 0;													\
	kernel_idt[0x##N].type_attr = 0x8E;											\
	kernel_idt[0x##N].offset_2 = ((uint32_t)&interrupt_entry_##N) >> 16;

#define MAKE_INTERRUPT_ENTRY_MANY(M)	\
	MAKE_INTERRUPT_ENTRY(M##0)			\
	MAKE_INTERRUPT_ENTRY(M##1)			\
	MAKE_INTERRUPT_ENTRY(M##2)			\
	MAKE_INTERRUPT_ENTRY(M##3)			\
	MAKE_INTERRUPT_ENTRY(M##4)			\
	MAKE_INTERRUPT_ENTRY(M##5)			\
	MAKE_INTERRUPT_ENTRY(M##6)			\
	MAKE_INTERRUPT_ENTRY(M##7)			\
	MAKE_INTERRUPT_ENTRY(M##8)			\
	MAKE_INTERRUPT_ENTRY(M##9)			\
	MAKE_INTERRUPT_ENTRY(M##A)			\
	MAKE_INTERRUPT_ENTRY(M##B)			\
	MAKE_INTERRUPT_ENTRY(M##C)			\
	MAKE_INTERRUPT_ENTRY(M##D)			\
	MAKE_INTERRUPT_ENTRY(M##E)			\
	MAKE_INTERRUPT_ENTRY(M##F)

	MAKE_INTERRUPT_ENTRY_MANY(0)
	MAKE_INTERRUPT_ENTRY_MANY(1)
	MAKE_INTERRUPT_ENTRY_MANY(2)
	MAKE_INTERRUPT_ENTRY_MANY(3)
	MAKE_INTERRUPT_ENTRY_MANY(4)
	MAKE_INTERRUPT_ENTRY_MANY(5)
	MAKE_INTERRUPT_ENTRY_MANY(6)
	MAKE_INTERRUPT_ENTRY_MANY(7)
	MAKE_INTERRUPT_ENTRY_MANY(8)
	MAKE_INTERRUPT_ENTRY_MANY(9)
	MAKE_INTERRUPT_ENTRY_MANY(A)
	MAKE_INTERRUPT_ENTRY_MANY(B)
	MAKE_INTERRUPT_ENTRY_MANY(C)
	MAKE_INTERRUPT_ENTRY_MANY(D)
	MAKE_INTERRUPT_ENTRY_MANY(E)
	MAKE_INTERRUPT_ENTRY_MANY(F)

	lidt(kernel_idt, sizeof(kernel_idt));

	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 4);
	outb(0xA1, 2);
	outb(0x21, 1);
	outb(0xA1, 1);
	outb(0x21, 0);
	outb(0xA1, 0);
	sti();
}

void interrupt_routine(int interrupt_num, pusha_t * regs, int errcode) {
	if (interrupt_handlers[interrupt_num]) {
		interrupt_handlers[interrupt_num](regs);
	} else {
		kprintf("Ignoring interrupt %x\n", interrupt_num);
	}
	if (interrupt_num >= 0x20 && interrupt_num < 0x30) {
		if (interrupt_num >= 0x28) {
			outb(0xA0, 0x20);
		}
		outb(0x20, 0x20);
	}
}
