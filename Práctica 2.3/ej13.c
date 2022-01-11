#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

volatile int stop = 0;

void handler(int signal) {
    if (signal == SIGUSR1)
        stop = 1;
}

int main(int argc, char *argv[]) {
    if(argc != 2){
		printf("Incorrect number of arguments\n");
		return 1;
	}

    sigset_t set;
	sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    int v = sigprocmask(SIG_UNBLOCK, &set, NULL);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    struct sigaction handle;

    v = sigaction(SIGUSR1, NULL, &handle);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    handle.sa_handler = handler;
    v = sigaction(SIGUSR1, &handle, NULL);
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    int secs = atoi(argv[1]);

    printf("Sleeping\n");
    sleep(secs);
    printf("Waking up\n");

    if (stop == 1) {
        printf("File saved\n");
    }
    else {
        printf("The file will be deleted\n");
        v = unlink(argv[0]);
        if (v == -1) {
            printf("Error: %d %s", errno, strerror(errno));
            return 1;
        }
        printf("I deleted myself! :(\n");
    }

    return 0;
}
