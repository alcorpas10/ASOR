#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Incorrect number of arguments\n");
		return 1;
	}
	
	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);
	if(fd == -1){
		printf("Error creating the file\n");
		return 1;
	}

	dup2(fd, STDOUT_FILENO);
	printf("Enviando cadena 1\n");
	printf("Enviando cadena 2\n");
	return 0;
}
