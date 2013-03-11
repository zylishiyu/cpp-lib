#include "apue.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

static const int MAXLINE = 1024;
static void err_doit(int errnoflag, int error, const char* fmt, va_list ap);

/**
* Fatal error related to a system call.
* Print a message and terminate.
*/
void err_sys(const char* fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

/**
* Fatal error unrelated to a system call.
* Error code passed as explicit paremeter.
* Print a message and terminate.
*/
void err_exit(int error, const char* fmt, ...) {
    va_list ap;
	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(1);
}

/**
 * Nonfatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

/**
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char* fmt, va_list ap) {
	char buf[MAXLINE];
	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag) {
		snprintf(buf + strlen(buf), MAXLINE - strlen(buf), ": %s", strerror(error));
	}
	strcat(buf, "\n");
	fflush(stdout);	/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);	/* flushes all stdio output streams */
}

