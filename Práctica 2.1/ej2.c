#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    if (setuid(0) == -1) {
        printf("Codigo: %d Cadena: %s\n", errno, strerror(errno));
    }
    return 1;
}