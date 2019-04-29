#define _GNU_SOURCE
#include <unistd.h>
#include <limits.h>
#include "schedule.h"
#include "process.h"

int get_highest_priority_process(int policy, int num_proc, struct process *procs, int now)
{
    int ret = -1;

    // TODO
    if (policy == FIFO) {
    }
    else if (policy == SJF) {
    }
    else if (policy == PSJF) {
    }
    else if (policy == RR) {
    }

    return ret;
}

void schedule(int policy, int num_proc, struct process *procs)
{
    int running = -1;
    int now = 0;

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
        UNIT_OF_TIME();
        ++now;
    }
}
