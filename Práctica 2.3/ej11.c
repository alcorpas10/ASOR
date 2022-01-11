#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTSTP);

    int v = sigprocmask(SIG_BLOCK, &set, NULL);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    printf("Sleeping\n");
    sleep(atoi(getenv("SLEEP_SECS")));
    printf("Waking up\n");

    sigset_t set_p;
    sigpending(&set_p);

    v = sigismember(&set_p, SIGINT);
    if (v == 1) {
        printf("SIGINT received\n");
        sigdelset(&set_p, SIGINT);
    }
    else {
        printf("SIGINT not received\n");
    }

    v = sigismember(&set_p, SIGTSTP);
    if (v == 1) {
        printf("SIGTSTP received\n");
        sigdelset(&set_p, SIGTSTP);
    }
    else {
        printf("SIGTSTP not received\n");
    }

    v = sigprocmask(SIG_UNBLOCK, &set, NULL);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    return 0;
}
