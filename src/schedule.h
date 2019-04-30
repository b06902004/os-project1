#ifndef _SCHEDULE_H
#define _SCHEDULE_H
#include "process.h"

enum Policy {
    FIFO = 0, RR, SJF, PSJF,
};

#define UNIT_OF_TIME()                      \
    do {                                    \
        volatile unsigned long i;           \
        for (i = 0; i < 1000000UL; ++i) {}  \
    } while(0);

#define PARENT_CPU 0
#define CHILD_CPU 1

void schedule(int policy, int num_proc, struct process *procs);
void cpu_assign(pid_t pid, int cpu);

#endif
