#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void io_wait(void) {
    __asm__ volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

static inline void lidt(void* base, uint16_t size)
{
    struct {
        uint16_t length;
        uint32_t base;
    } __attribute__((packed)) IDTR;
 
    IDTR.length = size;
    IDTR.base = (uint32_t) base;
    __asm__ ( "lidt (%0)" : : "p"(&IDTR) );
}

static inline void lgdt(void* base, uint16_t size) {
    struct {
        uint16_t length;
        uint32_t base;
    } __attribute__((packed)) GDTR;
 
    GDTR.length = size;
    GDTR.base = (uint32_t) base;
    __asm__ ( "lgdt (%0)" : : "p"(&GDTR) );
}

static inline void cli() {
	__asm__ ("cli");
}

static inline void sti() {
	__asm__ ("sti");
}

static inline void hlt() {
	__asm__ ("hlt");
}
