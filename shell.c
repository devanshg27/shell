#include <prompt.h>
#include <input.h>
#include <stdio.h>

void init(){
	initPrompt();
}

void del(){
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
			printf("%s\n", commands[position]);
			++position;
			// runCommand(commands[position]);
		}
	}
	del();
	return 0;
}