#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int pipe(int file_descriptor[2]);

int main(){
	int fd[2];
	assert(pipe(fd) != -1);

	pid_t pid = fork();
	assert(pid != -1);

	if(pid == 0){

		close(fd[1]);
		char buff[128] = {0};
		read(fd[0], buff, 127);
		printf(fd[0]);
	}

	else{
		close(fd[0]);
		write(fd[1], "hello", 5);
		close(fd[1]);
	}
	exit(0);

}

