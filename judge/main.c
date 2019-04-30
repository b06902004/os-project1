#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "../src/schedule.h"
#include "../src/process.h"

/* Subtract the ‘struct timeval’ values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */

struct timespec timespec_interval(struct timespec l, struct timespec r)
{
    const int BILLION = 1000000000;

    if (r.tv_nsec < l.tv_nsec) {
        int num_sec = (l.tv_nsec - r.tv_nsec) / BILLION + 1;
        l.tv_nsec -= BILLION * num_sec;
        l.tv_sec += num_sec;
    }
    if (r.tv_nsec - l.tv_nsec > BILLION) {
        int num_sec = (r.tv_nsec - l.tv_nsec) / BILLION;
        l.tv_nsec += BILLION * num_sec;
        l.tv_sec -= num_sec;
    }

    struct timespec ret;
    ret.tv_sec = r.tv_sec - l.tv_sec;
    ret.tv_nsec = r.tv_nsec - l.tv_nsec;
    assert(ret.tv_sec >= 0 && ret.tv_nsec >= 0);

    return ret;
}

int main(void)
{
    // TODO: Determine how long one unit of time is
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    UNIT_OF_TIME();
    clock_gettime(CLOCK_REALTIME, &end);
}
