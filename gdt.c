#include <stdint.h>
#include <stddef.h>

typedef uint8_t gdt_entry_t[8];
typedef uint8_t gdt_descriptor_t[6];

typedef struct __tss_struct {
    unsigned short   link;
    unsigned short   link_h;
    unsigned long   esp0;
    unsigned short   ss0;
    unsigned short   ss0_h;
    unsigned long   esp1;
    unsigned short   ss1;
    unsigned short   ss1_h;
    unsigned long   esp2;
    unsigned short   ss2;
    unsigned short   ss2_h;
    unsigned long   cr3;
    unsigned long   eip;
    unsigned long   eflags;
    unsigned long   eax;
    unsigned long   ecx;
    unsigned long   edx;
    unsigned long    ebx;
    unsigned long   esp;
    unsigned long   ebp;
    unsigned long   esi;
    unsigned long   edi;
    unsigned short   es;
    unsigned short   es_h;
    unsigned short   cs;
    unsigned short   cs_h;
    unsigned short   ss;
    unsigned short   ss_h;
    unsigned short   ds;
    unsigned short   ds_h;
    unsigned short   fs;
    unsigned short   fs_h;
    unsigned short   gs;
    unsigned short   gs_h;
    unsigned short   ldt;
    unsigned short   ldt_h;
    unsigned short   trap;
    unsigned short   iomap;
} tss_struct;

gdt_descriptor_t kernel_gdt_descriptor;
gdt_entry_t kernel_gdt[4];
tss_struct tss;

typedef struct gdt_entry_basic_t {
	uint32_t base;
	size_t limit;
	uint8_t type;
} gdt_entry_basic_t;

void encode_gdt_entry(uint8_t *target, gdt_entry_basic_t * source)
{
    // Check the limit to make sure that it can be encoded
    if ((source->limit > 65536) && (source->limit & 0xFFF) != 0xFFF) {
        //kerror("You can't do that!");
		return;		// suffer
    }
    if (source->limit > 65536) {
        // Adjust granularity if required
        source->limit = source->limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
 
    // Encode the limit
    target[0] = source->limit & 0xFF;
    target[1] = (source->limit >> 8) & 0xFF;
    target[6] |= (source->limit >> 16) & 0xF;
 
    // Encode the base 
    target[2] = source->base & 0xFF;
    target[3] = (source->base >> 8) & 0xFF;
    target[4] = (source->base >> 16) & 0xFF;
    target[7] = (source->base >> 24) & 0xFF;
 
    // And... Type
    target[5] = source->type;
}

void setup_flat_gdt() {
	gdt_entry_basic_t basic_gdt[4] = {
		{.base=0, .limit=0, .type=0},                     // Selector 0x00 cannot be used
		{.base=0, .limit=0xffffffff, .type=0x9A},         // Selector 0x08 will be our code
		{.base=0, .limit=0xffffffff, .type=0x92},         // Selector 0x10 will be our data
		{.base=(uint32_t)&tss, .limit=sizeof(tss), .type=0x89}  // You can use LTR(0x18)
	};

	encode_gdt_entry((uint8_t*) &kernel_gdt[0], &basic_gdt[0]);
	encode_gdt_entry((uint8_t*) &kernel_gdt[1], &basic_gdt[1]);
	encode_gdt_entry((uint8_t*) &kernel_gdt[2], &basic_gdt[2]);
	encode_gdt_entry((uint8_t*) &kernel_gdt[3], &basic_gdt[3]);

	*((uint16_t*)&kernel_gdt_descriptor[0]) = sizeof(kernel_gdt) - 1;
	*((void**)&kernel_gdt_descriptor[2]) = kernel_gdt;

	__asm__( "lgdt (%0)" : : "r" (&kernel_gdt_descriptor) );
	__asm__(	"ljmp $0x08, $complete_flush ;"

				"complete_flush:"
				"mov $0x10, %%ax ;"
				"mov %%ax, %%ds ;"
				"mov %%ax, %%es ;"
				"mov %%ax, %%fs ;"
				"mov %%ax, %%gs ;"
				"mov %%ax, %%ss ;"

				"mov $0x18, %%ax ;"
				"ltr %%ax ;"
			: : );
}
