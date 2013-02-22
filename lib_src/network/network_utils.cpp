#include "network_utils.h"

int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int nsec;

    /* 
     * try to connect with exponential backoff
     */
    for (nsec = 1; nsec <= NETWORK_UTILS_MAXSLEEP; nsec <<= 1) {
        if (connect(sockfd, addr, addrlen) == 0) {
            return 0;
        }
        if (nsec <= NETWORK_UTILS_MAXSLEEP/2) {
            sleep(nsec);
        }
    }
    return -1;
}

int init_server(int type, const struct sockaddr *addr, socklen_t addrlen, int backlog) {
    int fd;
    int err = 0;
    int flags;
    struct linger ling = {0, 0};

    if ((fd = socket(addr->sa_family, type, 0)) < 0) {
        //printf("create socket error! domain[%d] type[%d] protocol[%d]\n", addr->sa_family, type, 0);
        return -1;
    }
    if ((flags = fcntl(fd, F_GETFL, 0)) < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("setting O_NONBLOCK");
        return -1;
    }
    flags = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags)) != 0) {
        perror("setsockopt");
    }
    if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling)) != 0) {
        perror("setsockopt");
    }
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags)) != 0) {
        perror("setsockopt");
    }
    if (bind(fd, addr, addrlen) < 0) {
        err = errno;
        goto errout;
    }
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
        if (listen(fd, backlog) < 0) {
            err = errno;
            goto errout;
        }
    }
    return fd;

errout:
    close(fd);
    errno = err;
    return -1;
}

int init_tcp_server(int port, int backlog) {
    struct sockaddr_in server_addr;
    socklen_t server_addrlen;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addrlen = sizeof(server_addr);

    return init_server(SOCK_STREAM, (struct sockaddr*)&server_addr, server_addrlen, backlog);
}

