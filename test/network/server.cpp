#include "network_utils.h"

#include <string.h>

#define NETWORK_HOST_NAME_MAX 256

int process_request(int fd) {
    int client_sockfd;

    while (true) {
        char ch;
        client_sockfd = accept(fd, NULL, NULL);
        if (client_sockfd < 0) {
            if (errno == EAGAIN) {
                continue;
            }
            printf("accept error:%s", strerror(errno));
            return -1;
        }
        read(client_sockfd, &ch, 1);
        fprintf(stderr, "%c", ch);
        ++ch;
        write(client_sockfd, &ch, 1);
        close(client_sockfd);
    }
    return 0;
}

int main() {
    int port = 9999;
    int fd;

    if ((fd = init_tcp_server(port)) < 0) {
        printf("init tcp server failed! fd[%d] error[%s]\n", fd, strerror(errno));
        return -1;
    }
    process_request(fd);

    return 0;
}

