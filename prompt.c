#include <prompt.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

char *username;
char system_name[HOST_NAME_MAX + 1];
char home_directory[PATH_MAX + 1];

void initPrompt() {
	char *usernamePointer = getpwuid(geteuid())->pw_name;

	username = malloc(strlen(usernamePointer) + 1);
	if(username == NULL){
		perror("Malloc Failed\n");
		exit(0);
	}

	if(strcpy(username, usernamePointer) != NULL){
		int val = gethostname(system_name, HOST_NAME_MAX + 1);
		if(val == -1){
			perror("Error in getting hostname");
			exit(0);
		}
		if(getcwd(home_directory, PATH_MAX + 1) == NULL){
			perror("Getcwd error");
			exit(0);
		}
	}
	else{
		perror("Strcpy Error");
		exit(0);
	}

}

void showPrompt() {
	char current_directory[PATH_MAX + 1];
	int isPrefix = 1;
	
	if(getcwd(current_directory, PATH_MAX + 1) != NULL){
		for(int i=0; home_directory[i]!='\0'; i++) {
			if(home_directory[i] != current_directory[i]) {
				isPrefix = 0;
				break;
			}
		}
		printf("<%s@%s:", username, system_name);
		if(isPrefix) {
			printf("~%s> ", &current_directory[strlen(home_directory)]);
		}
		else {
			printf("%s> ", current_directory);
		}
	}
	else perror("getcwd() error");
}