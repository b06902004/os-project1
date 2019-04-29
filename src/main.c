#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "schedule.h"
#include "process.h"

int             policy;
int             num_proc;
struct process  *procs;

void read_input(void)
{
    char S[8];
    int N;

    scanf("%s", S);
    if (!strcmp(S, "FIFO")) {
        policy = FIFO;
    }
    else if (!strcmp(S, "RR")) {
        policy = RR;
    }
    else if (!strcmp(S, "SJF")) {
        policy = SJF;
    }
    else if (!strcmp(S, "PSJF")) {
        policy = PSJF;
    }
    else {
        fprintf(stderr, "read_input: S = %s\n", S);
        exit(1);
    }

    scanf("%d", &N);
    num_proc = N;

    procs = malloc(sizeof(struct process) * num_proc);
    assert(procs != NULL);
    for (int i = 0; i < num_proc; ++i) {
        scanf("%s %d %d", procs[i].name, &procs[i].ready_time, &procs[i].execution_time);
        procs[i].pid = -1;
    }
}

int main(void)
{
    read_input();
    schedule(policy, num_proc, procs);
}

