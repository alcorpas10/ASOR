#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Incorrect number of arguments");
        return 1;
    }

    struct addrinfo *res;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int v = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (v != 0) {
        perror("Error executing getaddrinfo");
        return 1;
    }

    int sfd = socket(res->ai_family, SOCK_DGRAM/*res->ai_socktype*/, res->ai_protocol);
    if (sfd == -1) {
        perror("Error opening the socket");
        return 1;
    }

    ssize_t writesize = sendto(sfd, argv[3], 2, 0, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
    if (writesize == -1) {
        perror("Error sending message");
        return 1;
    }

    char buf[257];
    ssize_t readsize = recvfrom(sfd, buf, 256, 0, (struct sockaddr *) res->ai_addr, &res->ai_addrlen);
    if (readsize == -1) {
        perror("Error receiving message");
        return 1;
    }

    buf[readsize] = '\0';
    printf("%s\n", buf);

    freeaddrinfo(res);
    
    close(sfd);
    return 0;
}