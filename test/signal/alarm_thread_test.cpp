#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

static void sig_alarm(int signo) {
    printf("sig alarm\n");
}

void thr_sig_block(int signo) {
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, signo);
    pthread_sigmask(SIG_BLOCK, &s, NULL);
}

void thr_sig_unblock(int signo) {
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, signo);
    pthread_sigmask(SIG_UNBLOCK, &s, NULL);
}

void* alarm_thread(void*) {
    thr_sig_block(SIGALRM);

    // send SIGALRM to thread that not block SIGALARM
    // thread scope alarm
    alarm(3);

    int unslept = sleep(10);
    printf("%s unslept: %d\n", __FUNCTION__, unslept);

    return NULL;
}

void* normal_thread(void*) {
    alarm(4);

    int unslept = sleep(10);
    printf("%s unslept: %d\n", __FUNCTION__, unslept);

    return NULL;
}

int main() {
    // set our handler, save previous information
    struct sigaction newact, oldact;
    sigset_t  newmask;
    newact.sa_handler = sig_alarm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    pthread_t alarm_thr;
    pthread_create(&alarm_thr, NULL, alarm_thread, NULL);

    pthread_t normal_thr;
    pthread_create(&normal_thr, NULL, normal_thread, NULL);

    pthread_join(alarm_thr, NULL);
    pthread_join(normal_thr, NULL);

    printf("exit main\n");

    return 0;
}

