#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sched.h>
#include <assert.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
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

int get_highest_priority_process(int policy, int num_proc, struct process *procs, int running,
                                 int now, int last_switch_time)
{
    int ret = -1;

    if (running != -1 && (policy == FIFO || policy == SJF))
        return running;

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
    else if (policy == SJF || policy == PSJF) {
        int min = INT_MAX;
        for (int i = 0; i < num_proc; ++i) {
            if (procs[i].pid == -1 || procs[i].execution_time == 0)
                continue;
            if (procs[i].execution_time < min) {
                min = procs[i].execution_time;
                ret = i;
            }
        }
    }
    else if (policy == RR) {
        if (running == -1) {
            for (int i = 0; i < num_proc; ++i) {
                if (procs[i].pid == -1 || procs[i].execution_time == 0)
                    continue;
                ret = i;
                break;
            }
        }
        else if ((now - last_switch_time) % 500 == 0) {
            ret = (running + 1) % num_proc;
            while (procs[ret].pid == -1 || procs[ret].execution_time == 0)
                ret = (ret + 1) % num_proc;
        }
        else {
            ret = running;
        }
    }

    return ret;
}


int cmp(const void *a, const void *b)
{
    return ((const struct process *)a)->ready_time - ((const struct process *)b)->ready_time;
}


void schedule(int policy, int num_proc, struct process *procs)
{
    int now = 0;
    int running = -1;
    int last_switch_time = 0;
    int num_finish = 0;

    cpu_assign(getpid(), PARENT_CPU);

    qsort(procs, num_proc, sizeof(struct process), cmp);

    while (1) {
        /* Check if the running process finishes now. If all the processes have
         * finished, discontinue the scheduler
         */
        if (running != -1 && procs[running].execution_time == 0) {
#ifdef DEBUG
            fprintf(stderr, "%s finishes at time %d\n", procs[running].name, now);
#endif
            waitpid(procs[running].pid, NULL, 0);
            running = -1;
            if (++num_finish == num_proc)
                break;
        }

        // Create those processes which are ready now
        for (int i = 0; i < num_proc; ++i) {
            if (procs[i].ready_time == now) {
#ifdef DEBUG
                fprintf(stderr, "%s is created at time %d\n", procs[i].name, now);
#endif
                procs[i].pid = process_create(procs[i]);
                process_sleep(procs[i]);
            }
        }

        // Get the index of the highest-priority process
        int next = get_highest_priority_process(policy, num_proc, procs, running, now,
                last_switch_time);
        // Context switch
        if (next != running) {
            if (running != -1)
                process_sleep(procs[running]);
            if (next != -1)
                process_run(procs[next]);
            last_switch_time = now;
        }
        running = next;

        // Pass one unit of time
        if (running != -1)
            --procs[running].execution_time;
        UNIT_OF_TIME();
        ++now;
    }
}
