#include <execute.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void runCommand(char *command){
	int BLOCK_SIZE = 100, BUFFER_SIZE = 100, position = 0;
	char **arguments = malloc(sizeof(char*) * BUFFER_SIZE);
	char *argument;

	argument = strtok(command, " \t");
	while(argument != NULL){
		arguments[position] = argument;
		++position;
		if(position == BUFFER_SIZE){
			BUFFER_SIZE += BLOCK_SIZE;
			arguments = realloc(arguments, BUFFER_SIZE * sizeof(char*));
		}
		argument = strtok(NULL, " \t");
	}
	arguments[position] = NULL;
	pid_t pid = fork();
	if(pid==0){
		execvp(arguments[0], arguments);
	}
	else{
		wait(NULL);
	}
}