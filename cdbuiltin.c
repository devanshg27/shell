#include <cdbuiltin.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int cd(char **arguments, int count, char *home_directory){
	if(count > 2){
		perror("Error: Invalid Usage\n");
		return 1;	
	} 
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
				return 1;
			}
			if(strcpy(destinationDirectory, home_directory) == NULL) {
				perror("Strcpy Error");
				return 1;
			}
			if(strcat(destinationDirectory, &arguments[1][1]) == NULL) {
				perror("Strcat Error");
				return 1;
			}
		}
		int val = chdir(destinationDirectory);
		if(val == -1){
			perror("Chdir Error");
			return 1;
		}
		return 0;
	} 
}