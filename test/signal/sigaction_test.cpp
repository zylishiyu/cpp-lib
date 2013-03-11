#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void loops(int outer_loops, int inner_loops) {
    for (int i = 0; i < outer_loops; ++i) {
        for (int j = 0; j < inner_loops; ++j) {
            int x = i * j;
        }
    }
}

static void sig_int(int signo) {
    // block all signals, include being processed signal
    printf("enter %s\n", __FUNCTION__);
    loops(1000 * 1000, 1000);
    printf("leave %s\n", __FUNCTION__);
}

void test_sig_int() {
    struct sigaction newact, oldact;
    sigset_t  newmask;

    // set our handler, save previous information
    newact.sa_handler = sig_int;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGINT, &newact, &oldact);

    loops(1000 * 1000, 1000);
}

int main() {
    test_sig_int();

    return 0;
}

