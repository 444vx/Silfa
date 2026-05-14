ASM = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc -Ikernel
LDFLAGS = -m elf_i386 -Ttext 0x8000 --oformat binary

all: silfa.bin

boot.bin: boot/boot.asm
	$(ASM) -f bin boot/boot.asm -o boot.bin

kernel.bin: kernel/kernel.c kernel/idt.c kernel/isr.asm
	$(ASM) -f elf32 kernel/isr.asm -o isr.o
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o
	$(LD) $(LDFLAGS) -e kernel_main -o kernel.bin kernel.o idt.o isr.o

silfa.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > silfa.bin
	truncate -s 1474560 silfa.bin

run:
	qemu-system-x86_64 -drive format=raw,file=silfa.bin

clean:
	rm -f *.o *.bin