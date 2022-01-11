#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

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

	printf("Major number: %d\n", major(stats.st_dev));
	printf("Minor number: %d\n", minor(stats.st_dev));

	printf("I-node: %d\n", stats.st_ino);

	mode_t mode = stats.st_mode;
	printf("%i\n",mode);
	if (S_ISLNK(mode))
		printf("Type: Symbolic link\n");
	else if (S_ISREG(mode))
		printf("Type: Regular file\n");
	else
		printf("Type: Directory\n");
	
	time_t t = stats.st_atime;
	char *date= ctime(&t);
	printf("Last access: %s\n", date);

	return 0;
}
