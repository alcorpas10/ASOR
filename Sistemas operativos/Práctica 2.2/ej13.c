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
	dup2(fd, STDERR_FILENO);

	printf("Enviando cadena 1 correcta\n");
	fprintf(stderr, "Enviando cadena 1 error\n");
	printf("Enviando cadena 2 correcta\n");
	fprintf(stderr, "Enviando cadena 2 error\n");
	return 0;
}
