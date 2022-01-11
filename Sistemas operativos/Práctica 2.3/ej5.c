#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

int main() {
    pid_t pid = getpid();
    printf("PID: %d\n", pid);
    
    pid_t ppid = getppid();
    printf("PPID: %d\n", ppid);
    
    pid_t pgid = getpgid(pid);
    if (pgid == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    printf("PGID: %d\n", pgid);

    pid_t sid = getsid(pid);
    if (sid == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    printf("SID: %d\n", sid);

    struct rlimit rlim;
    int lim = getrlimit(RLIMIT_NOFILE, &rlim);
    if (lim == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    printf("Max files: %d\n", rlim.rlim_max);

    char *path = malloc(sizeof(char)*(4096 + 1)); //Max length of a path in the system + \o
    getcwd(path, 4096 + 1);
    printf("CWD: %s\n", path);
    free(path);

    return 0;
}