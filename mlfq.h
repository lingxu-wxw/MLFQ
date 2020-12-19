#ifndef _H_MLFQ_
#define _H_MLFQ_

#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define INT_MAX 2147483647

const int PRIORITY_HIGH = 0;
const int PRIORITY_MID  = 1;
const int PRIORITY_LOW  = 2;

const int PRIORITIES = 3;

const int periods[3] = {2, 4, 8};

const int BOOST_PERIOD = 64;

const char* const colored_queues[3] = {"\033[32m0\033[0m", "\033[33m1\033[0m", "\033[36m2\033[0m"};

typedef struct _thread_attr {
    char *name;
    int   priority;
    int   quota;
    int   quotaremain;
    int   arrivetime;
    int   executetime;
    int   executeremain;
    int   (*job)(void *arg);
} thread_attr;

void boost();
void sched();

#endif
