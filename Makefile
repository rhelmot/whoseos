AS=i386-elf-as
CC=i386-elf-gcc

CFLAGS=-std=c11 -ffreestanding -O2 -Wall -Wextra

all: boot.o kernel.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

boot.o: boot.s
	$(AS) boot.s -o boot.o
