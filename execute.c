#include <execute.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <stdlib.h>
#include <grp.h>

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

void listFileInfo(char *directoryName, char *fileName){
	struct stat data;
	char filePath[PATH_MAX + 1];

	strcpy(filePath, directoryName);
	strcat(filePath, "/");
	strcat(filePath, fileName);
	stat(filePath, &data);
	if(S_ISDIR(data.st_mode)) printf("d");
	else if(S_ISCHR(data.st_mode)) printf("c");
	else if(S_ISLNK(data.st_mode)) printf("l");
	else if(S_ISFIFO(data.st_mode)) printf("p");
	else if(S_ISSOCK(data.st_mode)) printf("s");
	else if(S_ISBLK(data.st_mode)) printf("b");
	else printf("-");
	printf("%s", (data.st_mode & S_IRUSR) ? "r" : "-");
	printf("%s", (data.st_mode & S_IWUSR) ? "w" : "-");
	printf("%s", (data.st_mode & S_IXUSR) ? "x" : "-");
	printf("%s", (data.st_mode & S_IRGRP) ? "r" : "-");
	printf("%s", (data.st_mode & S_IWGRP) ? "w" : "-");
	printf("%s", (data.st_mode & S_IXGRP) ? "x" : "-");
	printf("%s", (data.st_mode & S_IROTH) ? "r" : "-");
	printf("%s", (data.st_mode & S_IWOTH) ? "w" : "-");
	printf("%s", (data.st_mode & S_IXOTH) ? "x" : "-");
	printf(" ");
	printf("%d ", data.st_nlink);
	printf("%s ", getpwuid(data.st_uid)->pw_name);
	printf("%s ", getgrgid(data.st_gid)->gr_name);
	printf("%10d ", data.st_size);

	char *c_time_string = ctime(&data.st_mtim.tv_sec);
	c_time_string[strlen(c_time_string) - 1] = '\0';
	printf("%s ", c_time_string);
}

void show(char *x, int l, int a){
	DIR* directory;
	struct dirent *current;
	directory = opendir(x);
	
	current = readdir(directory);

	for(; current != NULL; current = readdir(directory)){
		if((current->d_name)[0] == '.'){
			if(a){
				if(l){
					listFileInfo(x, current->d_name);
				}
				printf("%s\n", current->d_name);	
			} 
		}
		else{
			if(l){
				listFileInfo(x, current->d_name);
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

struct builtins{
	char *command;
	void (*commandFunction)(char **arguments, int count);
} implementedBuiltins[] = {{"cd", cd}, {"pwd", pwd}, {"echo", echo}, {"ls", ls}};

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
		if(strcmp(arguments[0], implementedBuiltins[i].command) == 0){
			(implementedBuiltins[i].commandFunction)(arguments, position);
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