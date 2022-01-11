#include <signal.h>
#include <errno.h>
#include <stdio.h>

volatile int cont_int = 0;
volatile int cont_tstp = 0;

void handler(int signal) {
    if (signal == SIGINT)
        cont_int++;
    if (signal == SIGTSTP)
        cont_tstp++;
}

int main() {
    struct sigaction handle;

    int v = sigaction(SIGINT, NULL, &handle);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    handle.sa_handler = handler;
    v = sigaction(SIGINT, &handle, NULL);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    
    v = sigaction(SIGTSTP, NULL, &handle);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    handle.sa_handler = handler;
    v = sigaction(SIGTSTP, &handle, NULL);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    sigset_t set;
	sigemptyset(&set);

	while ((cont_int + cont_tstp) < 10)
		sigsuspend(&set);

	printf("Num SIGINT: %d\n", cont_int);
	printf("Num SIGTSTP: %d\n", cont_tstp);

    return 0;
}
