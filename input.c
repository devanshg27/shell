#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokenize.h>

char **fetchCommands(){
	int BLOCK_SIZE = 100, BUFFER_SIZE = 100, position = 0, c = 0;
	char *inp = malloc(sizeof(char) * BUFFER_SIZE);
	while(1){
		read(0, &c, 1);
		if(c == '\n' || c == EOF) break;
		else{
			if(position == BLOCK_SIZE){
				BUFFER_SIZE += BLOCK_SIZE;
				inp = realloc(inp, BUFFER_SIZE);
			}
			inp[position] = c;
			++position;
		}
	}
	if(position == BLOCK_SIZE){
		++BUFFER_SIZE;
		inp = realloc(inp, BUFFER_SIZE); 
	}
	inp[position] = '\0';
	return tokenizeCommands(inp);
}

// void main(){
// 	initTermios();
// 	char **x = fetchCommands();
// 	int position = 0;
// 	while(1){
// 		if(x[position] == NULL) break;
// 		printf("%s\n", x[position]);
// 		++position;
// 	}
// 	resetTermios();
// }