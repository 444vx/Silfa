#include "process.h"

static process_t processes[MAX_PROCESSES];
static int process_count = 0;

void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].state = PROCESS_FREE;
    }
}

int process_create(void (*entry)(void)) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == PROCESS_FREE) {
            processes[i].state = PROCESS_READY;
            processes[i].entry = entry;
            process_count++;
            return i;
        }
    }
    return -1;
}