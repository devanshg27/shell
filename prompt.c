#include <prompt.h>
#include <limits.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

char *username;
char system_name[HOST_NAME_MAX + 1];
char home_directory[PATH_MAX + 1];

void initPrompt() {
	username = getpwuid(geteuid())->pw_name;
	gethostname(system_name, HOST_NAME_MAX + 1);
	getcwd(home_directory, PATH_MAX + 1);
}

void showPrompt() {
	char current_directory[PATH_MAX + 1];
	int isPrefix = 1;
	
	getcwd(current_directory, PATH_MAX + 1);
	
	for(int i=0; home_directory[i]!='\0'; i++) {
		if(home_directory[i] != current_directory[i]) {
			isPrefix = 0;
			break;
		}
	}
	printf("<%s@%s:", username, system_name);
	if(isPrefix) {
		printf("~> ", &current_directory[strlen(home_directory)]);
	}
	else {
		printf("%s> ", current_directory);
	}
}