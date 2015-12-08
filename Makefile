AS=i386-elf-as
CC=i386-elf-gcc

all: boot.o

boot.o: boot.s
	$(AS) boot.s -o boot.o
