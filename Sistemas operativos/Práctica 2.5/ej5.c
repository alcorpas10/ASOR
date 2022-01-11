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
#include <sys/select.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments\n");
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

    int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sfd == -1) {
        perror("Error opning the socket");
        return 1;
    }

    v = bind(sfd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
    if (v == -1) {
        printf("%d %s\n", errno, strerror(errno));
        close(sfd);
        return 1;
    }

    freeaddrinfo(res);

    int i;
    for (i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char buf[2], hostname[NI_MAXHOST], port[NI_MAXSERV];
            struct sockaddr_storage addr;
            socklen_t addrlen = sizeof(struct sockaddr_storage);
            int size = recvfrom(sfd, &buf, 2*sizeof(char), 0, (struct sockaddr *) &addr, &addrlen);
            if (size == -1) {
                perror("Error executing recvfrom");
                close(sfd);
                return 1;
            }

            v = getnameinfo((struct sockaddr *) &addr, addrlen, hostname, NI_MAXHOST, port, NI_MAXSERV, 0);
            if (v != 0) {
                perror("Error executing getnameinfo");
                return 1;
            }

            printf("Bytes: %d Name: %s Port: %s\n", size, hostname, port);
            
            if (buf[0] == 't') {
                time_t t;
                struct tm *loctime;
                char buffer[256];
                t =  time(NULL);
                loctime = localtime(&t);
                strftime(buffer, 256, "%r", loctime);
                int writesize = sendto(sfd, buffer, strlen(buffer), 0, (struct sockaddr *) &addr, addrlen);
                if (writesize == -1) {
                    perror("Error while sending back to client");
                    return -1;
                }
            } else if(buf[0] == 'd') {
                time_t t;
                struct tm *loctime;
                char buffer[256];
                t =  time(NULL);
                loctime = localtime(&t);
                strftime(buffer,256,"%Y-%m-%d", loctime);
                int writesize = sendto(sfd, buffer, strlen(buffer), 0, (struct sockaddr *) &addr, addrlen);
                if (writesize == -1) {
                    perror("Error while sending back to client");
                    return -1;
                }
            } else if (buf[0] == 'q') {
                close(sfd);
                return 0;
            } else
                printf("Error, unknown command %s", buf);
            return 0;
        }
    }
    while (wait(&status) > 0);
	
    close(sfd);
    return 0;
}
