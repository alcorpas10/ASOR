#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    struct addrinfo *res;
    /*struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;*/
    
    int v = getaddrinfo("147.96.1.9", NULL, NULL, &res);
    if (v != 0) {
        perror("Error executing getaddrinfo");
        return 1;
    }
    
    while (res != NULL){
        char host[NI_MAXHOST];
        getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        printf("%s %d %d\n", host, res->ai_family, res->ai_socktype);
        res = res->ai_next;
    }

    return 0;
}