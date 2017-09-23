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

typedef struct backgroundCommand {
	struct backgroundCommand* nextCommand;
	pid_t processId;
	int position;
} backgroundCommands;

backgroundCommands* startProcess = NULL;

void printJobs(backgroundCommands* iterator){
	if(iterator == NULL) return;

	printJobs(iterator->nextCommand);

	printf("[%d]\t", iterator->position);
	char pathToProcess[4096];
	sprintf(pathToProcess, "/proc/%d/wchan", iterator->processId);
	
	FILE* fp = fopen(pathToProcess, "r");
	if(fp != NULL){
		char status[4096];
		fscanf(fp, "%s", status);
	
		if(strcmp(status, "do_signal_stop") == 0) printf("Stopped\t");
		else printf("Running\t");

		fclose(fp);

		sprintf(pathToProcess, "/proc/%d/cmdline", iterator->processId);
		FILE* fp2 = fopen(pathToProcess, "r");

		if(fp2 != NULL){
			char processName[4096];
			fscanf(fp2, "%s", processName);

			if(strstr(processName, "/") == NULL) printf("\t%s [%d]\n", processName, iterator->processId);
			else printf("\t%s [%d]\n", 1 + strrchr(processName, (int)'/'), iterator->processId);

			fclose(fp2);
		}
	}	
}

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

	if(arguments[0][0] == 's'){
		if(position == 2) setenv(arguments[1], "", 1);	
		else if(position == 3) setenv(arguments[1], arguments[2], 1);
		else perror("Setenv: Invalid Usage\n");
		return 0;
	} 
	else if(arguments[0][0] == 'g'){
		if(position != 2) perror("Getenv: Invalid Usage");
		else printf("%s", getenv(arguments[1]));
		return 0;
	}

	if(arguments[0][0] == 'j'){
		printJobs(startProcess);
		return 0;
	}

	if(arguments[0][0] == 'u'){
		if(position == 2) unsetenv(arguments[1]	);
		else perror("Unsetenv: Invalid Usage");
	}

	if(arguments[0][0] == 'o'){
		backgroundCommands *iterator = startProcess, *temporary = startProcess;
		while(iterator){
			int val = kill(iterator->processId, SIGKILL);
			temporary = iterator->nextCommand;
			free(iterator);
			iterator = temporary;
		}
		startProcess = NULL;
		return 0;
	}

	pid_t PID = fork();

	if(PID == 0){
		setpgid(0, 0);
		execvp(arguments[0], arguments);
	}
	else{
		backgroundCommands* current = malloc(sizeof(backgroundCommands));
		
		if(startProcess == NULL) current->position = 1;
		else current->position = startProcess->position + 1;
		current->nextCommand = startProcess;
		current->processId = PID;
		
		startProcess = current;
	}
	return 0;
}