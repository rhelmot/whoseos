#include <stdint.h>
#include <stddef.h>

#include "lib.h"

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}

static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    __asm__ volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

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

uint8_t kernel_idt_descriptor[6];
idt_entry_t kernel_idt[256];

void (*interrupt_handlers[256])(pusha_t*);

void setup_interrupts() {
	for (int i = 0; i < 256; i++) {
		interrupt_handlers[i] = NULL;
	}

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

	*((uint16_t*)&kernel_idt_descriptor[0]) = sizeof(kernel_idt) - 1;
	*((void**)&kernel_idt_descriptor[2]) = kernel_idt;

	__asm__( "lidt (%0)" : : "r" (&kernel_idt_descriptor) );
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
	__asm__(	"sti" : : );
}

void interrupt_routine(int interrupt_num, pusha_t * regs) {
	if (interrupt_handlers[interrupt_num]) {
		interrupt_handlers[interrupt_num](regs);
	} else {
		kprintf("Ignoring interrupt %x\n", interrupt_num);
	}
}
