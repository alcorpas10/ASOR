#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(){
    long l = 0;
    if ((l = sysconf(_SC_ARG_MAX)) == -1){
        printf(strerror(errno));
    }
    else {
        printf("Max number of arguments: %li\n", l);
    }
    if ((l = sysconf(_SC_CHILD_MAX)) == -1){
        printf(strerror(errno));
    }
    else {
        printf("Max number of childs: %li\n", l);
    }
    if ((l = sysconf(_SC_OPEN_MAX)) == -1){
        printf(strerror(errno));
    }
    else {
        printf("Max number of open files: %li\n", l);
    }
    return 1;
}