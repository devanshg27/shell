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
#include <signal.h>

typedef struct backgroundCommand {
	struct backgroundCommand* nextCommand;
	char* commandName;
	pid_t processId;
	int position;
} backgroundCommands;

backgroundCommands* startProcess = NULL;

void childEndHandler(int sig){
	pid_t pid;
	int status;
	pid = wait(&status);

	backgroundCommands *iterator = startProcess, *prev = NULL;

	while(iterator) {
		if(iterator->processId == pid) {
			if(prev){
				prev->nextCommand = iterator->nextCommand;
			}
			else{
				startProcess = iterator->nextCommand;
			}
			break;
		}
		prev = iterator;
		iterator = iterator->nextCommand;
	}
	
	if(WIFEXITED(status)){
		int es = WEXITSTATUS(status);

		printf("%s with pid %d exited with status %d\n", iterator->commandName, pid, es);
	}

	free(iterator->commandName);
	free(iterator);
}

void initJobControl() {
	signal(SIGCHLD, childEndHandler);
}

int recursivePrintJob(backgroundCommands* iterator) {
	if(iterator == NULL) return 0;

	int val = recursivePrintJob(iterator->nextCommand);

	printf("[%d]\t", iterator->position);
	char pathToProcess[PATH_MAX+1];
	sprintf(pathToProcess, "/proc/%d/wchan", iterator->processId);
	
	FILE* fp = fopen(pathToProcess, "r");
	if(fp != NULL){
		char status[PATH_MAX+1];
		fscanf(fp, "%s", status);
	
		if(strcmp(status, "do_signal_stop") == 0) printf("Stopped\t");
		else printf("Running\t");

		fclose(fp);

		sprintf(pathToProcess, "/proc/%d/cmdline", iterator->processId);
		FILE* fp2 = fopen(pathToProcess, "r");

		if(fp2 != NULL){
			char processName[PATH_MAX+1];
			fscanf(fp2, "%s", processName);

			if(strstr(processName, "/") == NULL) printf("\t%s [%d]\n", processName, iterator->processId);
			else printf("\t%s [%d]\n", 1 + strrchr(processName, (int)'/'), iterator->processId);

			fclose(fp2);
		}
	}
	return val;
}

int printJobs(char **arguments, int count, char *home_directory){
	return recursivePrintJob(startProcess);
}

int setenvbuiltin(char **arguments, int count, char *home_directory){
	if(count == 2) setenv(arguments[1], "", 1);	
	else if(count == 3) setenv(arguments[1], arguments[2], 1);
	else {
		perror("Setenv: Invalid Usage\n");
		return 1;
	}
	return 0;
}

int getenvbuiltin(char **arguments, int count, char *home_directory){
	if(count != 2) {
		perror("Getenv: Invalid Usage");
		return 1;
	}
	else printf("%s", getenv(arguments[1]));
	return 0;
}

int unsetenvbuiltin(char **arguments, int count, char *home_directory){
	if(count == 2){
		unsetenv(arguments[1]);
	}
	else perror("Unsetenv: Invalid Usage");
	return 0;
}

int overkill(char **arguments, int count, char *home_directory){
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

void addToBackground(pid_t PID, char* commandArgument) {
	backgroundCommands* current = malloc(sizeof(backgroundCommands));
	if(startProcess == NULL) current->position = 1;
	else current->position = startProcess->position + 1;
	current->nextCommand = startProcess;
	current->processId = PID;
	current->commandName = malloc(sizeof(char) * (1 + strlen(commandArgument)));
	strcpy(current->commandName, commandArgument);
	startProcess = current;
}