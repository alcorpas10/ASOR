#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(){
    char *path = "/home/cursoredes/Desktop/SO/a.txt";
    long l = 0;
    if ((l = pathconf(path, _PC_LINK_MAX)) == -1){
        printf("%d", errno);
    }
    else {
        printf("Max number of links: %li\n", l);
    }
    if ((l = pathconf(path, _PC_PATH_MAX)) == -1){
        printf("%d", errno);
    }
    else {
        printf("Max length of path: %li\n", l);
    }
    if ((l = pathconf(path, _PC_NAME_MAX)) == -1){
        printf("%d", errno);
    }
    else {
        printf("Max length of file name: %li\n", l);
    }
    return 1;
}