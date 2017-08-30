#include <input.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <tokenize.h>

char **fetchCommands(){
	int BLOCK_SIZE = 100, BUFFER_SIZE = 100, position = 0;
	char c = 0;

	char *inp = malloc(sizeof(char) * BUFFER_SIZE);
	if(inp == NULL){
		perror("Malloc Failed\n");
		exit(0);
	}

	while(1){
		if(scanf("%c", &c) == EOF){
			puts("");
			exit(0);
		}
		else if(c == '\n') break;
		else{
			if(position == BLOCK_SIZE){
				BUFFER_SIZE += BLOCK_SIZE;
				inp = realloc(inp, BUFFER_SIZE);
				if(inp == NULL){
					perror("Realloc Error");
					exit(0);
				}
			}
			inp[position] = c;
			++position;
		}
	}
	if(position == BLOCK_SIZE){
		++BUFFER_SIZE;
		inp = realloc(inp, BUFFER_SIZE);
		if(inp == NULL){
			perror("Realloc Error");
			exit(0);
		} 
	}
	inp[position] = '\0';
	
	return tokenizeCommands(inp);
}
