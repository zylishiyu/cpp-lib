#ifndef __ZYLISHIYU_APUE_H__
#define __ZYLISHIYU_APUE_H__

void err_sys(const char* fmt, ...);
void err_quit(const char* fmt, ...);
void err_exit(int error, const char* fmt, ...);
void daemonize(const char* cmd);
int  already_running(void);

#endif  // __ZYLISHIYU_APUE_H__

