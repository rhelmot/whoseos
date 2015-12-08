AS=i386-elf-as
CC=i386-elf-gcc

CFLAGS=-std=c11 -ffreestanding -Wall -Wextra

all: kernel.bin bootable.iso

clean:
	rm *.o kernel.bin bootable.iso

kernel.bin: kernel.o boot.o linker.ld
	$(CC) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

boot.o: boot.s
	$(AS) boot.s -o boot.o

bootable.iso: kernel.bin
	mkdir -p image/boot/grub
	cp grub.cfg image/boot/grub
	cp kernel.bin image/boot
	grub-mkrescue -o bootable.iso image
	rm -rf image
