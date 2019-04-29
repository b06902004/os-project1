#ifndef _SCHEDULE_H
#define _SCHEDULE_H
#include "process.h"

enum Policy {
    FIFO = 0,
    RR,
    SJF,
    PSJF,
};

#define UNIT_OF_TIME()                      \
    do {                                    \
        volatile unsigned long i;           \
        for (i = 0; i < 1000000UL; ++i) {}  \
    } while(0);

void schedule(int policy, int num_proc, struct process *procs);

#endif
