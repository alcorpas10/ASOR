#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("Real UID: %d\nEffective UID: %d\n", uid, euid);
    return 1;
}