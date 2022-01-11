#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fd = open("a.txt", O_CREAT, 0645);
	if (fd == -1) {
		printf("Error creating the file\n");
		return 1;
	}
	return 0;
}

