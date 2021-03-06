#include "apue.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

static const char* LOCKFILE = "/var/run/daemon.pid";
static const int   LOCKMODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

void daemonize(const char* cmd) {
    int   i;
    int   fd0, fd1, fd2;
    pid_t pid;
    struct rlimit    rl;
    struct sigaction sa;

    /*
     * Clear file creation mask.
     */
    umask(0);

    /*
     * Get maximum number of file descriptors.
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        err_quit("%s: can't get file limit", cmd);
    }

    /*
     * Become a session leader to lose controlling TTY.
     */
    if ((pid = fork()) < 0) {
        err_quit("%s: can't fork", cmd);
    }else if (pid != 0) {
        exit(0);
    }
    setsid();

    /*
     * Ensure future opens won't allocate controlling TTYs.
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        err_quit("%s: can't ignore SIGHUP", cmd);
    }

    printf("daemonize ok\n");
    if ((pid = fork()) < 0) {
        err_quit("%s: can't fork", cmd);
    }else if (pid != 0) { /* parent */
        exit(0);
    }

    printf("daemonize ok ok\n");
    /*
     * Change the current working directory to the root so 
     * we won't prevent file systems from being unmounted.
     */
    if (chdir("/") < 0) {
        err_quit("%s: can't change directory to /");
    }

    printf("daemonize ok ok ok\n");
    /*
     * Close all open file descriptors.
     */
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    for (i = 0; i < (int)rl.rlim_max; i++) {
        close(i);
    }

    /*
     * Attach file descriptors 0, 1 and 2 to /dev/nul.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * Initialize the log file.
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d, %d, %d", fd0, fd1, fd2);
        exit(1);
    }
}

int lockfile(int fd) {
    struct flock    fl;

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return fcntl(fd, F_SETLK, &fl);
}

int already_running(void) {
    int  fd;
    char buf[16];

    umask(S_IWGRP | S_IWOTH);
    if ((fd = open(LOCKFILE, O_RDWR | O_CREAT), LOCKMODE) < 0) {
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return 1;
        }
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return 0;
}

