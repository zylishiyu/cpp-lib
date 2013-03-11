#include "thr_alarm.h"
#include <errno.h>
#include <linux/unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <queue>
#include <set>
#include <vector>

#define gettid() syscall(__NR_gettid)

struct thr_alarm_less {
    bool operator()(const thr_alarm_t& x, const thr_alarm_t& y) {
        return x.expire_time < y.expire_time;
    }
};

//static std::priority_queue<thr_alarm_t, std::vector<thr_alarm_t>, thr_alarm_less> g_alarms;
typedef std::set<thr_alarm_t, thr_alarm_less> Queue;
static Queue g_alarms;
static pthread_t alarm_thread;
static pthread_mutex_t g_alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_alarm_cond  = PTHREAD_COND_INITIALIZER;

unsigned long get_cur_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

static void sig_alarm(int signo) {
    fprintf(stdout, "thread:%d sig alarm\n", (int)gettid());
    fflush(stdout);
}

static int set_sig_handler(int signo, sighandler_t handler) {
    struct sigaction newact, oldact;
    newact.sa_handler = handler;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    if (signo == SIGALRM) {
        newact.sa_flags |= SA_INTERRUPT;
    } else {
        newact.sa_flags |= SA_RESTART;
    }
    int ret = sigaction(signo, &newact, &oldact);
    fprintf(stdout, "sigaction ret:%d", ret);
    return 0;
}
 
void* alarm_thr_func(void*) {
    int ret;
    ret = pthread_mutex_lock(&g_alarm_mutex);
    if (ret != 0) {
        fprintf(stderr, "mutex lock error! ret:%d", ret);
        fflush(stderr);
        return NULL;
    }
    unsigned long sleep_time;
    struct timespec sleep_timespec;
    while (true) {
        if (!g_alarms.empty()) {
            unsigned long cur_time = get_cur_time();
            Queue::iterator min_alarm = g_alarms.begin();
            while (cur_time >= min_alarm->expire_time) {
                fprintf(stdout, "pthread_kill %d", gettid());
                fflush(stdout);
                ret = pthread_kill(min_alarm->thread, SIGALRM);
                if (ret != 0) {
                    fprintf(stderr, "pthread_kill failed! ret:%d", ret);
                    fflush(stderr);
                }
                // modify expire_time to 3s later, wait to be deleted
                thr_alarm_t new_alarm = *min_alarm;
                new_alarm.expire_time += 3 * 1000 * 1000;
                g_alarms.erase(min_alarm);
                g_alarms.insert(new_alarm);
                min_alarm = g_alarms.begin();
            }
            if (!g_alarms.empty()) {
                min_alarm = g_alarms.begin();
                unsigned long sleep_time = min_alarm->expire_time - cur_time;
                sleep_timespec.tv_sec = sleep_time / 1000 * 1000;
                sleep_timespec.tv_nsec = sleep_time % (1000 * 1000) * 1000;
                ret = pthread_cond_timedwait(&g_alarm_cond, &g_alarm_mutex, &sleep_timespec);
                if (ret == ETIMEDOUT) {
                    fprintf(stdout, "pthread_cond_timedwait timed out");
                    fflush(stdout);
                } else {
                    fprintf(stdout, "pthread_cond_timedwait ret:%d", ret);
                    fflush(stdout);
                }
            }
        } else {
            ret = pthread_cond_wait(&g_alarm_cond, &g_alarm_mutex);
            fprintf(stdout, "pthread_cond_wait ret:%d", ret);
            fflush(stdout);
        }
    }
    ret = pthread_mutex_unlock(&g_alarm_mutex);
    if (ret != 0) {
        fprintf(stderr, "mutex unlock error! ret:%d", ret);
        fflush(stderr);
    }
    pthread_exit(0);
    return NULL;
}

int init_thr_alarm() {
    set_sig_handler(SIGALRM, sig_alarm);

    int ret = pthread_create(&alarm_thread, NULL, alarm_thr_func, NULL);
    if (ret != 0) {
        perror("create alarm thread fail!");
        return -1;
    }

    return 0;
}

void reschedule_alarms() {
    //int ret = pthread_kill(alarm_thread, SIGALRM);
    int ret = pthread_cond_signal(&g_alarm_cond);
    if (ret != 0) {
        fprintf(stderr, "pthread_kill failed! ret:%d", ret);
    }
}

int thr_alarm(unsigned long usecs) {
    thr_alarm_t cur_alarm;
    cur_alarm.expire_time = get_cur_time() + usecs;
    cur_alarm.tid = gettid();
    cur_alarm.thread = pthread_self();

    pthread_mutex_lock(&g_alarm_mutex);
    bool reschedule = g_alarms.empty() || (cur_alarm.expire_time < g_alarms.begin()->expire_time);
    g_alarms.insert(cur_alarm);
    pthread_mutex_unlock(&g_alarm_mutex);
    if (reschedule) {
        reschedule_alarms();
    }
    return 0;
}

int thr_del_alarm() {
    pid_t tid = gettid();
    pthread_mutex_lock(&g_alarm_mutex);
    for (Queue::iterator it = g_alarms.begin(); it != g_alarms.end(); ++it) {
        if (it->tid == tid) {
            bool is_top = (it == g_alarms.begin());
            g_alarms.erase(it);
            if (is_top) {
                reschedule_alarms();
            }
            fprintf(stderr, "thread:%d del alarm success", tid);
            break;
        }
    }
    pthread_mutex_unlock(&g_alarm_mutex);

    return 0;
}

