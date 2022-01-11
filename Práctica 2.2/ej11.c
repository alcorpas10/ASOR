#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
    		printf("Incorrect number of arguments\n");
    		return 1;
	}

	struct stat stats;
	int i = stat(argv[1], &stats);
    
	if (i == -1) {
    		printf("File error\n");
    		return 1;
	}

	if (!S_ISREG(stats.st_mode)) {
		printf("The file isn't a regular file\n");
    	return 1;
    }

	printf("Type: Regular file\n");

    char* hard = malloc(sizeof(char)*(5 + strlen(argv[1])));
	strcpy(hard, argv[1]);
	hard = strcat(hard, ".hard");
    int lnk = link(argv[1], hard);
    if (lnk == -1){
        printf("Error: %d %s\n", errno, strerror(errno));
    	return 1;
    }
    printf("Hard link created\n");
    
    char* sym = malloc(sizeof(char)*(5 + strlen(argv[1])));
    strcpy(sym, argv[1]);
    sym = strcat(sym, ".sym");
    int symlnk = symlink(argv[1], sym);
    if (symlnk == -1){
        printf("Error: %d %s\n", errno, strerror(errno));
    	return 1;
    }
    printf("Symbolic link created\n");

	return 0;
}
