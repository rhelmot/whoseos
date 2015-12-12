AS=i386-elf-as
CC=i386-elf-gcc

CFLAGS=-std=c11 -ffreestanding -Wall -Wextra

all: kernel.bin bootable.iso

clean:
	rm *.o kernel.bin bootable.iso

kernel.bin: kernel.o vga_text.o lib.o gdt.o boot.o interrupt.o interrupt_start.o keyboard.o linker.ld
	$(CC) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o vga_text.o lib.o gdt.o interrupt.o interrupt_start.o keyboard.o -lgcc

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o
vga_text.o: vga_text.c
	$(CC) $(CFLAGS) -c vga_text.c -o vga_text.o
lib.o: lib.c
	$(CC) $(CFLAGS) -c lib.c -o lib.o
gdt.o: gdt.c
	$(CC) $(CFLAGS) -c gdt.c -o gdt.o
interrupt.o: interrupt.c
	$(CC) $(CFLAGS) -c interrupt.c -o interrupt.o
keyboard.o: keyboard.c
	$(CC) $(CFLAGS) -c keyboard.c -o keyboard.o

interrupt_start.o: interrupt_start.s
	cpp interrupt_start.s | $(AS) -o interrupt_start.o

boot.o: boot.s
	$(AS) boot.s -o boot.o

bootable.iso: kernel.bin
	mkdir -p image/boot/grub
	cp grub.cfg image/boot/grub
	cp kernel.bin image/boot
	grub-mkrescue -o bootable.iso image
	rm -rf image
