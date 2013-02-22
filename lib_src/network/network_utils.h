#ifndef __ZYLISHIYU_NETWORK_UTILS_H_
#define __ZYLISHIYU_NETWORK_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>

#define NETWORK_UTILS_MAXSLEEP 2 /* s */

int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen);

int init_server(int type, const struct sockaddr *addr, socklen_t alen, int backlog);

int init_tcp_server(int port, int backlog = 1024);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __ZYLISHIYU_NETWORK_UTILS_H_

