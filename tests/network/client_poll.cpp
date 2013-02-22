#include "network_utils.h"

#include <string.h>
#include <poll.h>

#define NETWORK_CS_MAXADDRLEN 256

void init_addrinfo(struct addrinfo *aip, int ai_socktype) {
    aip->ai_flags = 0;
    aip->ai_family = 0;
    aip->ai_socktype = ai_socktype;
    aip->ai_protocol = 0;
    aip->ai_addrlen = 0;
    aip->ai_canonname = NULL;
    aip->ai_addr = NULL;
    aip->ai_next = NULL;
}

int io_wait(int sockfd) {
    int poll_timeout = 200;
    struct pollfd fds;
    fds.fd= sockfd;
    fds.events= POLLIN;
    int error = poll(&fds, 1, poll_timeout);
    switch (error) {
    case 1:
        return 0;
    case 0:
        printf("timeout\n");
        return -1;
    case -1:
    default:
        printf("poll error[%s]\n", strerror(errno));
        return -1;
    }
}

int send_request(int sockfd) {
    int n;
    const int BUF_LEN = 128;
    char buf[BUF_LEN];
    buf[0] = 'A';

    write(sockfd, buf, 1);
    do {
        int data_read = recv(sockfd, buf, BUF_LEN, MSG_DONTWAIT);
        if (data_read == -1) {
            switch (errno)
            {
            case EINTR:
                continue;

            //case ETIMEDOUT: // OSX
            //case EWOULDBLOCK:
            case EAGAIN:
            //case ERESTART:
                if (io_wait(sockfd) == 0)
                {
                    continue;
                }
                return 0;
            }
        }
        break;
    } while (true);
    return -1;
}

int main() {
    char *host = "10.81.31.67";
    int port = 9999;
    int sockfd;

    //struct addrinfo *ailist, *aip;
    //struct addrinfo hint;
    //char *service = NULL;
    //int err;
    //init_addrinfo(&hint, SOCK_STREAM);
    //if ((err = getaddrinfo(host, service, &hint, &ailist)) != 0) {
    //    printf("getaddrinfo error: %s", gai_strerror(err));
    //    return -1;
    //}
    //for (aip = ailist; aip != NULL; aip = aip->ai_next) {
    //    if ((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0) {
    //        err = errno;
    //        continue;
    //    }
    //    if (connect_retry(sockfd, aip->ai_addr, aip->ai_addrlen) < 0) {
    //        err = errno;
    //    } else {
    //        send_request(sockfd);
    //        return 0;
    //    }
    //}

    struct sockaddr_in server_addr;
    socklen_t server_addrlen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);
    server_addrlen = sizeof(server_addr);

    if (connect(sockfd, (struct sockaddr*)&server_addr, server_addrlen) != 0) {
        printf("connect failed!error[%s]\n", strerror(errno));
        return -1;
    }
    while (send_request(sockfd) == 0) {}

    return 0;
}

