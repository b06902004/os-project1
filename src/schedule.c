#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sched.h>
#include <assert.h>
#include "schedule.h"
#include "process.h"

void cpu_assign(pid_t pid, int cpu)
{
    assert(cpu == PARENT_CPU || cpu == CHILD_CPU);

    cpu_set_t mask;

    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);

    if (sched_setaffinity(pid, sizeof(cpu_set_t), &mask) < 0) {
        perror("sched_setaffinity");
        exit(1);
    }
}

int get_highest_priority_process(int policy, int num_proc, struct process *procs, int now)
{
    int ret = -1;

    // TODO
    if (policy == FIFO) {
        int min = INT_MAX;
        for (int i = 0; i < num_proc; ++i) {
            if (procs[i].pid == -1 || procs[i].execution_time == 0)
                continue;
            if (procs[i].ready_time < min) {
                min = procs[i].ready_time;
                ret = i;
            }
        }
    }
    else if (policy == SJF) {
    }
    else if (policy == PSJF) {
    }
    else if (policy == RR) {
    }

    return ret;
}

/*
int cmp(const void *a, const void *b)
{
    return ((const struct process *)a)->ready_time - ((const struct process *)b)->ready_time;
}
*/

void schedule(int policy, int num_proc, struct process *procs)
{
    int running = -1;
    int now = 0;

//    qsort(procs, num_proc, sizeof(struct process), cmp);

    cpu_assign(getpid(), PARENT_CPU);

    while (1) {
        // When every task's execution time = 0, break the for-loop
        int done = 1;
        for (int i = 0; i < num_proc; ++i) {
            if (procs[i].execution_time != 0) {
                done = 0;
                break;
            }
        }
        if (done) break;

        // Create those processes which are ready now
        for (int i = 0; i < num_proc; ++i) {
            if (procs[i].ready_time == now) {
                procs[i].pid = process_create(procs[i]);
                process_sleep(procs[i]);
            }
        }

        // Get the index of the highest-priority process
        int next = get_highest_priority_process(policy, num_proc, procs, now);
        if (next != running) {
            if (running != -1)
                process_sleep(procs[running]);
            process_run(procs[next]);
        }
        running = next;

        // Pass one unit of time
        if (running != -1)
            --procs[running].execution_time;
        fprintf(stderr, "running = %d\n", running);
        UNIT_OF_TIME();
        ++now;
    }
}
