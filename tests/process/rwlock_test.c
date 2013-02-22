#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "macro.h"

int rw_lock_test() {
    pthread_rwlock_t    rw_lock;
    pthread_rwlockattr_t attr; 
    int ret = 0;

    ret = pthread_rwlockattr_init(&attr); 
    if( ret < 0 ) {
        fprintf(stderr, "pthread_rwlockattr_init failed");
        return -1;
    }
    // prefer writer
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);

    ret = pthread_rwlock_init(&rw_lock, &attr); 
    if( ret < 0 ) {
        fprintf(stderr, "pthread_rwlock_init failed");
        return -1;
    }
    ret = pthread_rwlockattr_destroy(&attr); 
    if( ret < 0 ) {
        fprintf(stderr, "pthread_rwlockattr_destroy failed");
        return -1;
    }
    int loops = 1000 * 1000;
    int i;
    START_TIMER;
    for (i = 0; i < loops; ++i) {
        pthread_rwlock_rdlock(&rw_lock);
        pthread_rwlock_unlock(&rw_lock);
    }
    END_TIMER;

    return 0;
}

int main() {
    rw_lock_test();

    return 0;
}

