#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
    		printf("Incorrect number of arguments\n");
    		return 1;
	}
    
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    else if (pid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        
        execlp(argv[3], argv[3], argv[4], NULL);
    }
    else {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        
        execlp(argv[1], argv[1], argv[2], NULL);
    }
    return 0;
}