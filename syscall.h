#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_PRINT  0
#define SYSCALL_EXIT   1
#define SYSCALL_MALLOC 2
#define SYSCALL_FREE   3

void syscall_init(void);
int syscall_handler(int num, int arg1, int arg2, int arg3);

#endif