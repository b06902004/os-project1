#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include "process.h"
#include "schedule.h"

pid_t process_create(struct process proc)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid != 0) {  // parent
        return pid;
    }
    // Assign the child process to CPU_CHILD
    cpu_assign(getpid(), CHILD_CPU);


    proc.pid = getpid();
    printf("%s %d\n", proc.name, proc.pid);

    // TODO: show the start time via printk
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < proc.execution_time; ++i) {
        UNIT_OF_TIME();
        //fprintf(stderr, "process %s is running ...\n", proc.name);
    }

    // TODO: show the finish time via printk
    struct timespec finish;
    clock_gettime(CLOCK_REALTIME, &finish);

    exit(0);
}

void process_sleep(struct process proc)
{
    struct sched_param param;
    param.sched_priority = 0;
    if (sched_setscheduler(proc.pid, SCHED_IDLE, &param) < 0) {
        perror("sched_setscheduler");
        exit(1);
    }

}

void process_run(struct process proc)
{
    struct sched_param param;
    param.sched_priority = 0;
    sched_setscheduler(proc.pid, SCHED_OTHER, &param);
}
