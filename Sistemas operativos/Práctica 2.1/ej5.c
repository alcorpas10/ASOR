#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/utsname.h>

int main(){
    struct utsname buf;
    if (uname(&buf) == -1){
        printf(strerror(errno));
    }
    else {
        printf("Sysname: %s\nNodename: %s\nRelease: %s\nVersion: %s\nMachine: %s\n",
         buf.sysname, buf.nodename, buf.release, buf.version, buf.machine);
    }
    return 1;
}