#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

volatile int stop = 0;

void handler(int senial) {
    wait(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Incorrect number of arguments");
        return 1;
    }

    struct addrinfo *res;
    struct addrinfo hints;
    
    memset(&hints, 0, sizeof(struct addrinfo));
	
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
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

    v = listen(sfd, 5);
    if (v == -1) {
        printf("%d %s\n", errno, strerror(errno));
    	close(sfd);
        return 1;
    }

    int status;
    char buf[80], host[NI_MAXHOST], port[NI_MAXSERV];
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(struct sockaddr_storage);

    signal(SIGCHLD, handler);

    while(1) {
        int nsfd = accept(sfd, (struct sockaddr *) &addr, &addrlen);
        if (v == -1) {
            printf("%d %s\n", errno, strerror(errno));
            close(sfd);
            return 1;
        }

        pid_t pid = fork();

        if (pid == 0) {
            v = getnameinfo((struct sockaddr *) &addr, addrlen, host, NI_MAXHOST, port, NI_MAXSERV, 0);
            if (v != 0) {
                perror("Error executing getnameinfo");
                close(nsfd);
                close(sfd);
                return 1;
            }

            printf("Connected to -> Name: %s Port: %s\n", host, port);

            ssize_t size = recv(nsfd, buf, 80, 0);
            while (size > 0) {
                size = send(nsfd, buf, size, 0);
                if (size < 1) {
                    printf("%d %s\n", errno, strerror(errno));
                    close(nsfd);
                    close(sfd);
                    return 1;
                }
                size = recv(nsfd, buf, 80, 0);
            }
            if (size == 0)
                printf("Client disconnected\n");
        } else {
            //pid = wait(&status);
            close(nsfd);
        }
    }
    
    close(sfd);
    return 0;
}
