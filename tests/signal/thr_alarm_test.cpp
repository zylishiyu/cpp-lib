#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "macro.h"
#include "thr_alarm.h"

void* worker_thread(void*) {
    // set timer
    unsigned long alarm_time = 10 * 1000;
    thr_alarm(alarm_time);

    fprintf(stdout, "thr_alarm %d", alarm_time);
    fflush(stdout);

    // block
    START_TIMER;
    sigset_t block_set;
    sigfillset(&block_set);
    sigdelset(&block_set, SIGALRM);
    sigsuspend(&block_set);
    END_TIMER;

    // del timer
    // thr_del_alarm
    thr_del_alarm();

    fprintf(stderr, "worker thread return");
    fflush(stderr);
    return NULL;
}

int main() {
    init_thr_alarm();

    pthread_t t;
    pthread_create(&t, NULL, worker_thread, NULL);
    pthread_join(t, NULL);

    return 0;
}

