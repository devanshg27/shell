#include <prompt.h>
#include <input.h>
#include <stdio.h>
#include <execute.h>
#include <unistd.h>
#include <stdlib.h>

void init(){
	initPrompt();
}

int main(){
// char *args[] = {"/bin/ls", "-la", (char *) 0 };
// execvp("/bin/ls", args);

	init();
	while(1){
		showPrompt();
		int val = fflush(stdout);
		if(val != 0){
			perror("Error in Flushing Output\n");
			exit(0);
		}
		char **commands = fetchCommands();
		int position = 0;
		while(1){
			if(commands[position] == NULL) break;
			runCommand(commands[position]);
			++position;
		}
	}

	return 0;
}