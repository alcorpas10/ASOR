#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
        perror("Incorrect number of arguments\n");
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
		perror("Error opening the socket");
        return 1;
	}

    v = connect(sfd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
    if (v == -1) {
		printf("%d %s\n", errno, strerror(errno));
        close(sfd);
        return 1;
	}

	freeaddrinfo(res);

    char buf[80];
    ssize_t writesize, readsize;
    readsize = read(STDIN_FILENO, buf, 79);
    if (readsize == -1) {
        printf("%d %s\n", errno, strerror(errno));
        close(sfd);
        return 1;
    }
    buf[readsize] = '\0';
    while(strcmp("Q\n", buf)) {
        writesize = send(sfd, buf, readsize+1, 0);
        if (writesize < 1) {
            printf("%d %s\n", errno, strerror(errno));
            close(sfd);
            return 1;
        }
        readsize = recv(sfd, buf, 79, 0);
        if (readsize == -1) {
			printf("%d %s\n", errno, strerror(errno));
            close(sfd);
            return 1;
		}
		buf[readsize] = '\0';
        printf("%s", buf);
        readsize = read(STDIN_FILENO, buf, 79);
        if (readsize == -1) {
            printf("%d %s\n", errno, strerror(errno));
            close(sfd);
            return 1;
        }
        buf[readsize] = '\0';
    }

    close(sfd);
    return 0;
}
