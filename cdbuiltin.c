#include <cdbuiltin.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void cd(char **arguments, int count, char *home_directory){
	if(count > 2) printf("Error: Invalid Usage\n");
	else{
		char *destinationDirectory;
		if(count == 1) destinationDirectory = home_directory;
		else destinationDirectory = arguments[1];
		if(count > 1 && strcmp(arguments[1], "~") == 0) {
			destinationDirectory = home_directory;
		}
		else if(count > 1 && arguments[1][0] == '~' && arguments[1][1] == '/') {
			destinationDirectory = malloc(strlen(home_directory) + strlen(arguments[1]));
			if(destinationDirectory == NULL) {
				perror("Malloc Failed");
				exit(0);
			}
			if(strcpy(destinationDirectory, home_directory) == NULL) {
				perror("Strcpy Error");
				exit(0);
			}
			if(strcat(destinationDirectory, &arguments[1][1]) == NULL) {
				perror("Strcat Error");
				exit(0);
			}
		}
		int val = chdir(destinationDirectory);
		if(val == -1){
			perror("Chdir Error");
			exit(0);
		}
	} 
}