#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char *path1 = "/home/cursoredes/tuberia1";
    char *path2 = "/home/cursoredes/tuberia2";
    char buf[256];
    int v = mkfifo(path1, 0777);
    if (v == -1) {
        printf("Error: %d %s\n", errno, strerror(errno));
        return 1;
    }

    v = mkfifo(path2, 0777);
    if (v == -1) {
        printf("Error: %d %s\n", errno, strerror(errno));
        return 1;
    }

    int fd1 = open(path1, O_RDONLY | O_NONBLOCK);
    int fd2 = open(path2, O_RDONLY | O_NONBLOCK);

    fd_set readfds;
    int fdin, n;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);

        if (fd1 < fd2)
            fdin = fd2+1;
        else
            fdin = fd1+1;       
        int fd = select(fdin, &readfds, NULL, NULL, NULL);
        
        if (FD_ISSET(fd1, &readfds)){
            n = read(fd1, buf, 255);
            buf[n] = '\0';
            printf("Pipe 1: %s", buf);
            if (n < 255) {
                close(fd1);
                fd1 = open(path1, O_RDONLY | O_NONBLOCK);
            }
        }
        else {
            n = read(fd2, buf, 255);
            buf[n] = '\0';
            printf("Pipe 2: %s", buf);
            if (n < 255) {
                close(fd2);
                fd2 = open(path2, O_RDONLY | O_NONBLOCK);
            }
        }
    }

    close(fd1);
    close(fd2);

    return 0;
}