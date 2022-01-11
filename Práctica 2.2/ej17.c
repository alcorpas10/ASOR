#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
    	printf("Incorrect number of arguments\n");
    	return 1;
	}
    
	DIR *directory = opendir(argv[1]);
    
	if (directory == NULL) {
    	printf("Incorrect argument\n");
    	return 1;
	}
    
    struct dirent *dir = readdir(directory);

	if (dir == NULL) {
    	printf("Error: %d %s\n", errno, strerror(errno));
    	return 1;
	}
    
    int size = 0;
    int dirSize = sizeof(char)*strlen(argv[1]);

    while (dir != NULL) {
        char *path = malloc(dirSize + sizeof(char)*(strlen(dir->d_name) + 1));
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dir->d_name);

        struct stat stats;
        int i = stat(path, &stats);
    
        if (i == -1) {
            printf("File error\n");
            free(path);
            closedir(directory);
            return 1;
        }

        if (S_ISREG(stats.st_mode))
            printf("Type: Regular file\nName: %s", dir->d_name);
        else if (S_ISDIR(stats.st_mode))
            printf("Type: Directory\nName: %s\\", dir->d_name);
        else if (S_ISLNK(stats.st_mode)) {
            char *buf = malloc(stats.st_size + 1);
            int e = readlink(path, buf, stats.st_size + 1);

            if (e == -1){
                printf("Error: %d %s", errno, strerror(errno));
                free(path);
                free(buf);
                closedir(directory);
    	        return 1;
            }
            printf("Type: Symbolic link\nName: %s->%s", dir->d_name, buf);
            free(buf);
        }
        if (stats.st_mode & S_IXUSR)
            printf("*\n\n");
        else
            printf("\n\n");

        if (!S_ISDIR(stats.st_mode))
            size += stats.st_blocks/2;
        free(path);
        dir = readdir(directory);
    }
    closedir(directory);
    
    printf("Total size of files: %d KB\n", size);
	return 0;
}
