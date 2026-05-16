#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 16
#define STACK_SIZE    4096

typedef enum {
    PROCESS_FREE,
    PROCESS_READY,
    PROCESS_RUNNING
} process_state_t;

typedef struct {
    unsigned int pid;
    process_state_t state;
    unsigned int esp;
    unsigned int stack[STACK_SIZE];
    void (*entry)(void);
} process_t;

void process_init(void);
int  process_create(void (*entry)(void));
void process_yield(void);

#endif