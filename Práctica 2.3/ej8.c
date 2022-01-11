#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
    		printf("Incorrect number of arguments\n");
    		return 1;
	}

    pid_t pid = fork();
    if (pid == 0) {
        // Hijo
        pid_t sid = setsid();
        if (sid == -1) {
            printf("Error: %d %s", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        int fd_out = open("/tmp/daemon.out", O_CREAT | O_RDWR, 0777);
        if(fd_out == -1){
            printf("Error creating the file\n");
            close(fd_out);
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);

        int fd_err = open("/tmp/daemon.err", O_CREAT | O_RDWR, 0777);
        if(fd_err == -1){
            perror("Error creating the file\n");
            close(fd_out);
            close(fd_err);
            exit(EXIT_FAILURE);
        }
        dup2(fd_err, STDERR_FILENO);

        int fd_in = open("/dev/null", O_CREAT | O_RDWR, 0777);
        if(fd_in == -1){
            printf("Error creating the file\n");
            close(fd_out);
            close(fd_err);
            close(fd_in);
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);

        int x = execvp(argv[1], argv + 1);
        if (x == -1) {
            printf("Error executing execvp\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    /*else if (pid > 0) {
        // Padre
        int status;
        wait(&status);
        if (status == -1) {
            printf("Error in child");
            return 1;
        }
    }*/
    else if (pid == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    return 0;
}