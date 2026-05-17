ASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc -Ikernel
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

all: silfa.bin

boot.bin: boot/boot.asm
	$(ASM) -f bin boot/boot.asm -o boot.bin

kernel.bin: kernel/entry.asm kernel/kernel.c kernel/idt.c kernel/keyboard.c kernel/mm.c kernel/process.c kernel/fs.c kernel/syscall.c kernel/paging.c kernel/tss.c kernel/shell.c kernel/disk.c kernel/sfs.c kernel/elf.c kernel/isr.asm
	$(ASM) -f elf32 kernel/entry.asm -o entry.o
	$(ASM) -f elf32 kernel/isr.asm -o isr.o
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o
	$(CC) $(CFLAGS) -c kernel/keyboard.c -o keyboard.o
	$(CC) $(CFLAGS) -c kernel/mm.c -o mm.o
	$(CC) $(CFLAGS) -c kernel/process.c -o process.o
	$(CC) $(CFLAGS) -c kernel/fs.c -o fs.o
	$(CC) $(CFLAGS) -c kernel/syscall.c -o syscall.o
	$(CC) $(CFLAGS) -c kernel/paging.c -o paging.o
	$(CC) $(CFLAGS) -c kernel/tss.c -o tss.o
	$(CC) $(CFLAGS) -c kernel/shell.c -o shell.o
	$(CC) $(CFLAGS) -c kernel/disk.c -o disk.o
	$(CC) $(CFLAGS) -c kernel/sfs.c -o sfs.o
	$(CC) $(CFLAGS) -c kernel/elf.c -o elf.o
	$(LD) $(LDFLAGS) -e _start -o kernel.bin entry.o kernel.o idt.o isr.o keyboard.o mm.o process.o fs.o syscall.o paging.o tss.o shell.o disk.o sfs.o elf.o

silfa.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > silfa.bin
	truncate -s 1474560 silfa.bin

run:
	qemu-system-x86_64 -drive format=raw,file=silfa.bin

clean:
	rm -f *.o *.bin