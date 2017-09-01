#include <pwdbuiltin.h>
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>

void pwd(char **arguments, int count, char *home_directory){
	if(count > 1) printf("Error: Invalid Usage\n");
	char present_directory[PATH_MAX + 1];
	if(getcwd(present_directory, PATH_MAX + 1) != NULL){
		printf("%s\n", present_directory);
	}
	else{
		perror("Getcwd Error");
		exit(0);
	}
}