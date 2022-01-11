#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main(){
    time_t t = time(NULL);
    if (t == -1){
        printf(strerror(errno));
    }
    else{
        printf("Seconds passed from epoch: %ld\n", t);
    }
    return 1;
}