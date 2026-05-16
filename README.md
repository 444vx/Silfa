# Silfa Kernel

A kernel built from scratch in C and x86 Assembly. No Unix, no BSD, no Linux — everything is original.

## Features

- Custom bootloader
- Protected mode (x86 32-bit)
- Interrupt handling (IDT, PIC)
- PS/2 keyboard driver
- Memory allocator (kmalloc/kfree)
- Paging (virtual memory foundation)
- Round-robin scheduler
- RAM filesystem
- SilfaFS - persistent filesystem surviving reboots
- System calls
- Interactive shell

## Shell commands

- help - show commands
- ls - list files
- create <name> - create file
- write <name> <text> - write to file
- read <name> - read file
- delete <name> - delete file
- echo <text> - print text
- clear - clear screen
- version - show version

## Requirements

- NASM
- GCC (32-bit support)
- GNU ld
- QEMU

## Build and run

make clean && make && make run

## Philosophy

Silfa is built with security and privacy in mind. Every application runs in a sandbox — no process can access another process memory. No antivirus needed because the sandbox replaces it. No kernel-level anticheats allowed.

## Status

Early development. Contributions welcome.