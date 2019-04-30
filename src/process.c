#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <sys/syscall.h>
#include "process.h"
#include "schedule.h"
#define PRINTK 548
#define GET_NSTIME 549

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

    // Record the start time
    struct timespec start;
    syscall(GET_NSTIME, &start);
    /* clock_gettime(CLOCK_REALTIME, &start); */

    for (int i = 0; i < proc.execution_time; ++i) {
        UNIT_OF_TIME();
    }

    // Record the finish time
    struct timespec finish;
    syscall(GET_NSTIME, &finish);
    /* clock_gettime(CLOCK_REALTIME, &finish); */

    // Show the information
    char buf[1024] = {};
    sprintf(buf, "[project1] %d %ld.%.9ld %ld.%.9ld\n", proc.pid, start.tv_sec, start.tv_nsec,
            finish.tv_sec, finish.tv_nsec);
    syscall(PRINTK, buf);
    /* printf("[project1] %d %ld.%.9ld %ld.%.9ld\n", proc.pid, start.tv_sec, start.tv_nsec,
            finish.tv_sec, finish.tv_nsec); */

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
