ASM = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc
LDFLAGS = -m elf_i386 -Ttext 0x8000 --oformat binary

all: silfa.bin

boot.bin: boot/boot.asm
	$(ASM) -f bin boot/boot.asm -o boot.bin

kernel.bin: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o
	$(LD) $(LDFLAGS) -e kernel_main -o kernel.bin kernel.o

silfa.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > silfa.bin

run:
	qemu-system-x86_64 -drive format=raw,file=silfa.bin

clean:
	rm -f *.o *.bin