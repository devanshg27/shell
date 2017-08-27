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
		char **x = fetchCommands();
		int position = 0;
		while(1){
			if(x[position] == NULL) break;
			printf("%s\n", x[position]);
			++position;
		}
	}
	del();
	return 0;
}