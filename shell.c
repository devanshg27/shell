#include <prompt.h>
#include <input.h>
#include <stdio.h>
#include <execute.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void init(){
	initPrompt();
	initExecute();
}

int main(){
	init();
	while(1){
		int error = showPrompt();
		if(error) continue;
		int val = fflush(stdout);
		if(val != 0){
			perror("Error in Flushing Output");
			continue;
		}
		char **commands = fetchCommands();
		int position = 0;
		while(1){
			if(commands[position] == NULL) break;
			int val = runCommand(commands[position]);
			++position;
		}
	}

	return 0;
}