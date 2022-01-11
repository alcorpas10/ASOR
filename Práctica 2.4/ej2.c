#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int p_h[2];
    pipe(p_h);

    int h_p[2];
    pipe(h_p);

    pid_t pid = fork();
    if (pid == -1) {
        printf("Error: %d %s", errno, strerror(errno));
        return 1;
    }
    else if (pid == 0) {
        close(p_h[1]);
        close(h_p[0]);

        char buf[512];
        char response[1] = {'l'};

        int i;
        for (i = 0; i < 10; i++) {
            ssize_t size = read(p_h[0], buf, 511);
            printf("Received '%s' from my parent\n", buf);
            memset(buf, 0, sizeof(buf));


            sleep(1);

            if (i == 9)
                response[0] = 'q';
            
            write(h_p[1], response, 1);
        }

        close(p_h[0]);
        close(h_p[1]);
    }
    else {
        close(p_h[0]);
        close(h_p[1]);

        char buf[512];
        char response[1];

        while(response[0] != 'q') {
            printf("Write a message for my child:\n");
            ssize_t size = read(0, buf, 512);

            write(p_h[1], buf, size-1);
            memset(buf, 0, sizeof(buf));

            read(h_p[0], response, 1);
        }
        
        wait(NULL);

        close(p_h[1]);
        close(h_p[0]);
    }
    return 0;
}