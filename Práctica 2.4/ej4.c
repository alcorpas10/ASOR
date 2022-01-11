#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments\n");
    	return 1;
    }
    char *path = "/home/cursoredes/tuberia1";
    int v = mkfifo(path, 0777);
    if (v == 0) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    int fd = open(path, O_WRONLY);
    v = write(fd, argv[1], strlen(argv[1]));
    if (v == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }

    close(fd);

    return 0;
}
