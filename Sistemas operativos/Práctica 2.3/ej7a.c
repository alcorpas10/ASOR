#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
    		printf("Incorrect number of arguments\n");
    		return 1;
	}

    int x = system(argv[1]);
    if (x == -1) {
        printf("Error executing system\n");
        return 1;
    }

    printf("El comando termino de ejecutarse\n");
    return 0;
}