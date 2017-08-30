#include <prompt.h>
#include <input.h>
#include <stdio.h>
#include <execute.h>
#include <unistd.h>

void init(){
	initPrompt();
}

int main(){
// char *args[] = {"/bin/ls", "-la", (char *) 0 };
// execvp("/bin/ls", args);

	init();
	while(1){
		showPrompt();
		fflush(stdout);
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