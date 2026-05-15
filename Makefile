ASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc -Ikernel
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

all: silfa.bin

boot.bin: boot/boot.asm
	$(ASM) -f bin boot/boot.asm -o boot.bin

kernel.bin: kernel/entry.asm kernel/kernel.c kernel/idt.c kernel/keyboard.c kernel/isr.asm
	$(ASM) -f elf32 kernel/entry.asm -o entry.o
	$(ASM) -f elf32 kernel/isr.asm -o isr.o
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o
	$(CC) $(CFLAGS) -c kernel/keyboard.c -o keyboard.o
	$(LD) $(LDFLAGS) -e _start -o kernel.bin entry.o kernel.o idt.o isr.o keyboard.o

silfa.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > silfa.bin
	truncate -s 1474560 silfa.bin

run:
	qemu-system-x86_64 -drive format=raw,file=silfa.bin

clean:
	rm -f *.o *.bin