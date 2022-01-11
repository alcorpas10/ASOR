#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	umask(0027);
	int fd = open("a.txt", O_CREAT, 0645);
	if (fd == -1) {
		printf("Error creating the file\n");
		return 1;
	}
	return 0;
}
