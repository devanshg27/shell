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
#include <jobcontrol.h>
#include <pinfo.h>
#include <errno.h>

char home_directory[PATH_MAX + 1];

void initExecute(){
	if(getcwd(home_directory, PATH_MAX + 1) == NULL){
		perror("Getcwd error");
		exit(0);
	}
}

typedef struct {
	char *inputFile;
	char **arguments;
	char *outputFile;
} commandObject;

struct builtins{
	char *command;
	int (*commandFunction)(char **arguments, int count, char *home_directory);
} implementedBuiltins[] = {
	{"cd", cd},
	{"pwd", pwd},
	{"echo", echo},
	{"ls", ls},
	{"nightswatch", nightswatch},
	{"pinfo", pinfo},
	{"jobs", printJobs},
	{"setenv", setenvBuiltin},
	{"getenv", getenvBuiltin},
	{"unsetenv", unsetenvBuiltin},
	{"overkill", overkill},
	{"quit", quitBuiltin}
};

int convertTilde(char *dest, char *argument) {
	if(strcpy(dest, argument) == NULL) {
		fprintf(stderr, "Strcpy Error\n");
		return 1;
	}
	if(strcmp(argument, "~") == 0) {
		if(strcpy(dest, home_directory) == NULL) {
			fprintf(stderr, "Strcpy Error\n");
			return 1;
		}
	}
	else if(argument[0] == '~' && argument[1] == '/') {
		if(strcpy(dest, home_directory) == NULL) {
			fprintf(stderr, "Strcpy Error\n");
			return 1;
		}
		if(strcat(dest, &argument[1]) == NULL) {
			fprintf(stderr, "Strcat Error\n");
			return 1;
		}
	}
	return 0;
}

int commandParser(char *command, commandObject** listCommands) {
	int BUFFER_SIZE = BLOCK_SIZE, position = 0;

	*listCommands = malloc(sizeof(commandObject) * BUFFER_SIZE);
	if(*listCommands == NULL){
		perror("Malloc Failed");
		return 1;
	}

	char *argument;

	argument = strtok(command, "|");
	while(argument != NULL){
		(*listCommands)[position].inputFile = argument;
		++position;
		if(position == BUFFER_SIZE){
			BUFFER_SIZE += BLOCK_SIZE;
			*listCommands = realloc(*listCommands, BUFFER_SIZE * sizeof(commandObject));
			if(*listCommands == NULL){
				perror("Realloc Error");
				return 1;
			}
		}
		argument = strtok(NULL, "|");
	}

	(*listCommands)[position].inputFile = NULL;
	(*listCommands)[position].arguments = NULL;
	(*listCommands)[position].outputFile = NULL;

	int positionArgument;
	for (int i=0; i<position; ++i) {
		positionArgument = 0;
		BUFFER_SIZE = BLOCK_SIZE;
		argument = (*listCommands)[i].inputFile;
		(*listCommands)[i].inputFile = NULL;
		(*listCommands)[i].outputFile = NULL;
		(*listCommands)[i].arguments = malloc(BUFFER_SIZE * sizeof(char *));
		if((*listCommands)[i].arguments == NULL){
			perror("Malloc Failed");
			return 1;
		}
		char *pch = strtok(argument, " \t");
		while(pch != NULL){
			if(strcmp(pch, "<") == 0) {
				pch = strtok(NULL, " \t");
				if(pch) {
					(*listCommands)[i].inputFile = pch;
				}
				else {
					fprintf(stderr, "Syntax Error\n");
					return 1;
				}
			}
			else if(strcmp(pch, ">") == 0) {
				pch = strtok(NULL, " \t");
				if(pch) {
					(*listCommands)[i].outputFile = pch;
				}
				else {
					fprintf(stderr, "Syntax Error\n");
					return 1;
				}
			}
			else {
				(*listCommands)[i].arguments[positionArgument] = pch;
				++positionArgument;
				if(positionArgument == BUFFER_SIZE){
					BUFFER_SIZE += BLOCK_SIZE;
					(*listCommands)[i].arguments = realloc((*listCommands)[i].arguments, BUFFER_SIZE * sizeof(char *));
					if(*listCommands == NULL){
						perror("Realloc Error");
						return 1;
					}
				}
			}
			pch = strtok(NULL, " \t");
		}
		(*listCommands)[i].arguments[positionArgument] = NULL;
		for (int j = 0; j<positionArgument; ++j) {
			argument = (*listCommands)[i].arguments[j];
			(*listCommands)[i].arguments[j] = malloc(sizeof(char) * (strlen(argument) > PATH_MAX ? strlen(argument) : PATH_MAX));
			if((*listCommands)[i].arguments[j] == NULL) {
				perror("Malloc Failed");
				return 1;
			}
			if(convertTilde((*listCommands)[i].arguments[j], argument)) return 1;
		}
		if((*listCommands)[i].inputFile) {
			argument = (*listCommands)[i].inputFile;
			(*listCommands)[i].inputFile = malloc(sizeof(char) * PATH_MAX);
			if(convertTilde((*listCommands)[i].inputFile, argument)) return 1;
		}
		if((*listCommands)[i].outputFile) {
			argument = (*listCommands)[i].outputFile;
			(*listCommands)[i].outputFile = malloc(sizeof(char) * PATH_MAX);
			if(convertTilde((*listCommands)[i].outputFile, argument)) return 1;
		}
	}
	return 0;
}

int runCommand(char *command){
	int commandLength = strlen(command);
	int isBackground = 0;
	if(commandLength > 1 && command[commandLength-1] == '&') {
		command[commandLength-1] = '\0';
		isBackground = 1;
		--commandLength;
	}
	commandObject *listCommands;
	if(commandParser(command, &listCommands)) return 1;
	for(int i=0; listCommands[i].arguments!=NULL; ++i) {
		// printf("Command ");
		// if(listCommands[i].inputFile) {
		// 	printf("< %s ", listCommands[i].inputFile);
		// }
		// if(listCommands[i].outputFile) {
		// 	printf("> %s ", listCommands[i].outputFile);
		// }
		// puts("");
		// for (int j = 0; listCommands[i].arguments[j]!=NULL; ++j) {
		// 	printf("\t%s\n", listCommands[i].arguments[j]);
		// }

		int numberOfImplementedBuiltins = sizeof(implementedBuiltins)/sizeof(implementedBuiltins[0]);
		for(int j=0; j<numberOfImplementedBuiltins; ++j){
			if(strcmp(listCommands[i].arguments[0], implementedBuiltins[j].command) == 0){
				int countArguments = 0;
				while(listCommands[i].arguments[countArguments]) ++countArguments;
				int val = (implementedBuiltins[j].commandFunction)(listCommands[i].arguments, countArguments, home_directory);
				return val;
			}
		}

		pid_t PID = fork();

		if(PID == 0){
			setpgid(0, 0);
			execvp(listCommands[i].arguments[0], listCommands[i].arguments);
			perror("Execvp Error");
			exit(0);
		}
		else{
			addToBackground(PID, listCommands[i].arguments[0]);
		}
	}

	for(int i=0; listCommands[i].arguments!=NULL; ++i) {
		for (int j = 0; listCommands[i].arguments[j]!=NULL; ++j) {
			free(listCommands[i].arguments[j]);
		}
		if(listCommands[i].inputFile) free(listCommands[i].inputFile);
		free(listCommands[i].arguments);
		if(listCommands[i].outputFile) free(listCommands[i].outputFile);
	}
	free(listCommands);
	return 0;
}