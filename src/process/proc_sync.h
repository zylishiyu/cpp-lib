#ifndef __ZYLISHIYU_PROC_SYNC_H_
#define __ZYLISHIYU_PROC_SYNC_H_

#include <unistd.h>

void TELL_WAIT(void);

void TELL_PARENT(pid_t pid);

void WAIT_PARENT(void);

void TELL_CHILD(pid_t pid);

void WAIT_CHILD(void);

#endif  // __ZYLISHIYU_PROC_SYNC_H_

