#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
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

    proc.pid = getpid();
    printf("%s %d\n", proc.name, proc.pid);

    // TODO: show the start time via printk
    for (int i = 0; i < proc.execution_time; ++i) {
        UNIT_OF_TIME();
    }
    // TODO: show the finish time via printk

    exit(0);
}

void process_sleep(struct process proc)
{
    struct sched_param param;
    param.sched_priority = 0;
    sched_setscheduler(proc.pid, SCHED_IDLE, &param);
}

void process_run(struct process proc)
{
    struct sched_param param;
    param.sched_priority = 0;
    sched_setscheduler(proc.pid, SCHED_OTHER, &param);
}
