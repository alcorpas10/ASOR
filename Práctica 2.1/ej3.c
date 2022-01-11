#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(){
    int i = 0;
    char* s = "";
    for (i = 0; i < 135; i++) {
        printf("Codigo %d: Cadena: %s\n", i, s);
        s = strerror(i);
    }
    return 1;
}
