#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

static void sig_alarm(int signo) {
    printf("sig alarm\n");
}

void* ualarm_test(void* id) {
    printf("thread:%d started\n", (int)id);

    //sleep((int)id + 1);
    unsigned int usecs = 1000 * 1000;
    //ualarm(usecs, 0);
    ualarm(((int)id + 1) * usecs, 0);
    int i = sleep(30);
    printf("id:%d unslept:%d\n", (int)id, i);

    printf("thread:%d end\n", (int)id);
}

int main() {
    // set our handler, save previous information
    struct sigaction newact, oldact;
    sigset_t  newmask;
    newact.sa_handler = sig_alarm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    //ualarm_test(NULL);
    const int thread_num = 10;
    pthread_t t[thread_num];
    for (int i = 0; i < thread_num; ++i) {
        pthread_create(&t[i], NULL, ualarm_test, (void*)i);
    }
    for (int i = 0; i < thread_num; ++i) {
        pthread_join(t[i], NULL);
    }

    printf("exit main\n");

    return 0;
}
