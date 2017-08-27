#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **tokenizeCommands(char *input){
	int BLOCK_SIZE = 100, BUFFER_SIZE = 100, position = 0;
	char **tokens = malloc(sizeof(char*) * BUFFER_SIZE);
	char *token;

	token = strtok(input, ";");
	while(token != NULL){
		tokens[position] = token;
		++position;
		if(position == BUFFER_SIZE){
			BUFFER_SIZE += BLOCK_SIZE;
			tokens = realloc(tokens, BUFFER_SIZE * sizeof(char*));
		}

		token = strtok(NULL, ";");
	}
	tokens[position] = NULL;
	return tokens;
}
