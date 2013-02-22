#include "network_utils.h"

#include <string.h>

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

void send_request(int sockfd) {
    int n;
    const int BUF_LEN = 128;
    char buf[BUF_LEN];
    buf[0] = 'A';

    write(sockfd, buf, 1);
    while ((n = recv(sockfd, buf, BUF_LEN, 0)) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
    if (n < 0) {
        printf("recv error [%s]", strerror(errno));
        return;
    }
    if (n == 0) {
        printf("\n\n%d recv all message.\n", sockfd);
        return;
    }
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

    if (connect_retry(sockfd, (struct sockaddr*)&server_addr, server_addrlen) != 0) {
        printf("connect failed!error[%s]\n", strerror(errno));
        return -1;
    }
    send_request(sockfd);

    return 0;
}

