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

int showPrompt() {
	char current_directory[PATH_MAX + 1];
	int isPrefix = 1;
	
	if(getcwd(current_directory, PATH_MAX + 1) != NULL){
		for(int i=0; home_directory[i]!='\0'; i++) {
			if(home_directory[i] != current_directory[i]) {
				isPrefix = 0;
				break;
			}
		}
		printf("<\033[0;36m%s\033[0m@\033[0;32m%s\033[0m:\033[1;33m", username, system_name);
		if(isPrefix) {
			printf("~%s\033[0m> ", &current_directory[strlen(home_directory)]);
		}
		else {
			printf("%s\033[0m> ", current_directory);
		}
		return 0;
	}
	else{
		perror("getcwd() error");
		return 1;	
	} 
}