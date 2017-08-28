#include <prompt.h>
#include <input.h>
#include <stdio.h>
#include <execute.h>

void init(){
	initPrompt();
}

int main(){
	init();
	while(1){
		showPrompt();
		fflush(stdout);
		char **commands = fetchCommands();
		int position = 0;
		
		while(1){
			if(commands[position] == NULL) break;
			// printf("%s\n", commands[position]);
			runCommand(commands[position]);
			++position;
		}
	}
	return 0;
}