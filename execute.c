#include <execute.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

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
	if(count > 2) printf("Error\n");
	else chdir(arguments[1]);
}

void pwd(char **arguments, int count){
	if(count > 1) printf("Error\n");
	char present_directory[PATH_MAX + 1];
	getcwd(present_directory, PATH_MAX + 1);
	printf("%s\n", present_directory);
}

void echo(char **arguments, int count){
	for(int i=1; i<count; ++i){
		printf("%s", arguments[i]);
		if(i < count-1) printf(" ");
	}
	printf("\n");
}

void show(char *x, int l, int a){
	DIR* directory;
	struct dirent *current;
	directory = opendir(x);
	
	current = readdir(directory);

	struct stat data;


	for(; current != NULL; current = readdir(directory)){
		if((current->d_name)[0] == '.'){
			if(a){
				if(l){
					stat(current->d_name, &data);
					if(S_ISREG(data.st_mode)) printf("-");
					else printf("d");
					if(data.st_mode & S_IRUSR) printf("r");
					else printf("-");
					if(data.st_mode & S_IWUSR) printf("w");
					else printf("-");
					if(data.st_mode & S_IXUSR) printf("x");
					else printf("-");
					if(data.st_mode & S_IRGRP) printf("r");
					else printf("-");
					if(data.st_mode & S_IWGRP) printf("w");
					else printf("-");
					if(data.st_mode & S_IXGRP) printf("x");
					else printf("-");
					if(data.st_mode & S_IROTH) printf("r");
					else printf("-");
					if(data.st_mode & S_IWOTH) printf("w");
					else printf("-");
					if(data.st_mode & S_IXOTH) printf("x");
					else printf("-");
					printf("\t");
					
				}
				printf("%s\n", current->d_name);	
			} 
		}
		else{
			if(l){
				stat(current->d_name, &data);
				if(S_ISREG(data.st_mode)) printf("-");
				else printf("d");
				if(data.st_mode & S_IRUSR) printf("r");
				else printf("-");
				if(data.st_mode & S_IWUSR) printf("w");
				else printf("-");
				if(data.st_mode & S_IXUSR) printf("x");
				else printf("-");
				if(data.st_mode & S_IRGRP) printf("r");
				else printf("-");
				if(data.st_mode & S_IWGRP) printf("w");
				else printf("-");
				if(data.st_mode & S_IXGRP) printf("x");
				else printf("-");
				if(data.st_mode & S_IROTH) printf("r");
				else printf("-");
				if(data.st_mode & S_IWOTH) printf("w");
				else printf("-");
				if(data.st_mode & S_IXOTH) printf("x");
				else printf("-");
				printf("\t");
				
			}
			printf("%s\n", current->d_name);				
		} 
	}

}

void ls(char **arguments, int count){
	int A = 0, L = 0, idx = 1;
	char *location = malloc(sizeof(char) * 4096);

	while(arguments[idx] != NULL){
		if(arguments[idx][0] == '-' && strlen(arguments[idx]) > 1){
			for(int j=1; j<strlen(arguments[idx]); ++j){
				if(arguments[idx][j] == 'a') A = 1;
				else if(arguments[idx][j] == 'l') L = 1;
				else{
					printf("Invalid Flag");
					return;
				}
			}
		}
		++idx;
	}


	int yes = 0; idx = 1;
	while(arguments[idx] != NULL){
		if(arguments[idx][0] == '-'){
			if((int)strlen(arguments[idx]) > 1);
			else{
				show(arguments[idx], L, A);
				yes = 1;
			}
		}
		else{
			show(arguments[idx], L, A);
			yes = 1;			
		}
		++idx;
	}

	if(yes == 0){
		char *current = malloc(sizeof(char) * 2);
		current = ".";
		show(current, L, A);
	}
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
			(implementedFunctions[i])(arguments, position);
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