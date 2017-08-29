#include <execute.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void runCommandinBackground(char **arguments) {
	pid_t pid = fork();
	if(pid==0){
		execvp(arguments[0], arguments);
	}
	else{
		wait(NULL);
		fprintf(stderr, "%s with pid %d exited normally\n", arguments[0], pid);
		exit(0);
	}
}

void cd(char **arguments, int count){
	if(count != 2) printf("Error\n");
	else chdir(arguments[1]);
}

void pwd(char **arguments, int count){

}

void echo(char **arguments, int count){

}

void ls(char **arguments, int count){

}

void (*implementedFunctions[10])(char **arguments, int count);
char **implemented;

void executeInit(){

	implemented = malloc(sizeof(char*) * 10);

	implementedFunctions[0] = cd;
	implemented[0] = "cd";

	implementedFunctions[1] = pwd;
	implemented[1] = "pwd";

	implementedFunctions[2] = echo;
	implemented[2] = "echo";

	implementedFunctions[3] = ls;
	implemented[3] = "ls";
}

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

	for(int i=0; i<4; ++i){
		if(strcmp(arguments[0], implemented[i]) == 0){
			(*implementedFunctions)(arguments, position);
			return;
		}
	}

	arguments[position] = NULL;
	if(strcmp(arguments[position-1], "&") == 0) {
		--position;
		arguments[position] = NULL;
		pid_t pid = fork();
		if(pid==0){
			runCommandinBackground(arguments);
		}
	}
	else{
		pid_t pid = fork();
		if(pid==0){
			execvp(arguments[0], arguments);
		}
		else{
			wait(NULL);
		}
	}
}