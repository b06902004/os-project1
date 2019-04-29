#ifndef _PROCESS_H
#define _PROCESS_H
#include <sys/types.h>
#include <unistd.h>

#define NAME_SIZE 40

struct process {
    char name[NAME_SIZE];
    int ready_time;
    int execution_time;

    // Initialized to -1, meaning that this process has not been executed
    pid_t pid;
};

pid_t process_create(struct process proc);
void process_sleep(struct process proc);
void process_run(struct process proc);

#endif
