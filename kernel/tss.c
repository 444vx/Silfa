#include "tss.h"

static tss_t tss;

void tss_init(unsigned int kernel_stack) {
    tss.ss0  = 0x10;
    tss.esp0 = kernel_stack;
    tss.cs   = 0x0B;
    tss.ss   = 0x13;
    tss.ds   = 0x13;
    tss.es   = 0x13;
    tss.fs   = 0x13;
    tss.gs   = 0x13;
}

void tss_set_stack(unsigned int esp0) {
    tss.esp0 = esp0;
}