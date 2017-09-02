#include <execute.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cdbuiltin.h>
#include <pwdbuiltin.h>
#include <echobuiltin.h>
#include <lsbuiltin.h>
#include <nightswatch.h>
#include <pinfo.h>
#include <errno.h>

char home_directory[PATH_MAX + 1];

void initExecute(){
	if(getcwd(home_directory, PATH_MAX + 1) == NULL){
		perror("Getcwd error");
		exit(0);
	}
}

int runCommandinBackground(char **arguments) {
	pid_t pid = fork();
	if(pid == -1){
		perror("Fork Error");
		return 1;
	}
	else if(pid==0){
		execvp(arguments[0], arguments);
		perror("Execvp Error");
		exit(0);
	}
	else{

		int status;

		if(waitpid(pid, &status, 0) == -1){
			fprintf(stderr, "waitpid failed\n");
			exit(0);
		}
		if(WIFEXITED(status)){
			int es = WEXITSTATUS(status);
			printf("%s with pid %d exited with status %d\n", arguments[0], pid, es);
			exit(0);
		}
	}
	return 0;
}

struct builtins{
	char *command;
	int (*commandFunction)(char **arguments, int count, char *home_directory);
} implementedBuiltins[] = {{"cd", cd}, {"pwd", pwd}, {"echo", echo}, {"ls", ls}, {"nightswatch", nightswatch}, {"pinfo", pinfo}};

int runCommand(char *command){

	int BUFFER_SIZE = 100, position = 0;

	char **arguments = malloc(sizeof(char*) * BUFFER_SIZE);
	if(arguments == NULL){
		perror("Malloc Failed");
		return 1;
	}

	char *argument;

	argument = strtok(command, " \t");
	while(argument != NULL){
		arguments[position] = argument;
		++position;
		if(position == BUFFER_SIZE){
			BUFFER_SIZE += BLOCK_SIZE;
			arguments = realloc(arguments, BUFFER_SIZE * sizeof(char*));
			if(arguments == NULL){
				perror("Realloc Error");
				return 1;
			}
		}
		argument = strtok(NULL, " \t");
	}

	arguments[position] = NULL;

	if(position > 1 && strcmp(arguments[position-1], "&") == 0) {
		--position;
		arguments[position] = NULL;
		pid_t pid = fork();
		if(pid == -1){
			perror("Fork Error");
			free(arguments);
			return 1;
		}
		else if(pid == 0){
			int numberOfImplementedBuiltins = sizeof(implementedBuiltins)/sizeof(implementedBuiltins[0]);
			for(int i=0; i<numberOfImplementedBuiltins; ++i){
				if(strcmp(arguments[0], implementedBuiltins[i].command) == 0){
					int val = (implementedBuiltins[i].commandFunction)(arguments, position, home_directory);
					printf("%s with pid %d exited with status %d\n", arguments[0], (int) getpid(), val);
					exit(0);
				}
			}
			runCommandinBackground(arguments);
			exit(0);
		}
		else{
			free(arguments);
		}
	}
	else{
		int numberOfImplementedBuiltins = sizeof(implementedBuiltins)/sizeof(implementedBuiltins[0]);
		for(int i=0; i<numberOfImplementedBuiltins; ++i){
			if(strcmp(arguments[0], implementedBuiltins[i].command) == 0){
				int val = (implementedBuiltins[i].commandFunction)(arguments, position, home_directory);
				free(arguments);
				return val;
			}
		}
		pid_t pid = fork();
		if(pid == -1){
			perror("Fork Error");
			free(arguments);
			return 1;
		}
		else if(pid == 0){
			execvp(arguments[0], arguments);
			perror("Execvp Error");
			exit(0);
		}
		else{
			int status = 0;
			int val = waitpid(pid, &status, WUNTRACED);
			free(arguments);
			if(val == -1){
				fprintf(stderr, "Wait Error\n");
				return 1;
			}
		}
	}
	return 0;
}