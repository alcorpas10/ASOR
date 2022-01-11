#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

int processAttribs() {
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

    char *path = malloc(sizeof(char)*(4096 + 1));
    getcwd(path, 4096 + 1);
    printf("CWD: %s\n", path);
    free(path);
}

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Soy el hijo\n");
        
        pid_t sid = setsid();
        if (sid == -1) {
            printf("Error: %d %s", errno, strerror(errno));
            return 1;
        }

        int dir = chdir("/tmp");
        if (dir == -1) {
            printf("Error: %d %s", errno, strerror(errno));
            return 1;
        }

        processAttribs();
        exit(0);
    }
    else if (pid > 0) {
        printf("Soy el padre\n");
        processAttribs();

        int status;
        wait(&status);
        if (status == -1) {
            printf("Error in child");
            return 1;
        }
    }
    else if (pid == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    return 0;
}