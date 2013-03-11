#include <stdio.h>
#include <sys/time.h>

static long long ustime(void) {
    struct timeval tv;
    long long ust;

    gettimeofday(&tv, NULL);
    ust = ((long)tv.tv_sec)*1000000;
    ust += tv.tv_usec;
    return ust;
}   


void func() {
}

int main() {
    int loops = 1000 * 1000 * 1000;
    long long cost_time = ustime();
    for (int i = 0; i < loops; ++i) {
        func();
    }
    cost_time = ustime() - cost_time;
    long long loop_time = ustime();
    for (int i = 0; i < loops; ++i) {
    }
    loop_time = ustime() - loop_time;
    cost_time -= loop_time;
    printf("function call cost time [%f] ns\n", (cost_time / (double)loops) * 1000);

    return 0;
}

