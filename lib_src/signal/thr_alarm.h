#ifndef  __ZYLISHIYU_THREAD_ALARM_H_
#define  __ZYLISHIYU_THREAD_ALARM_H_

#include <pthread.h>
#include <sys/types.h>

typedef struct thr_alarm_t {
    unsigned long expire_time;
    pid_t tid;
    pthread_t thread;
} thr_alarm_t;

int init_thr_alarm();

int thr_alarm(unsigned long usecs);

int thr_del_alarm();


#endif  // __ZYLISHIYU_THREAD_ALARM_H_

