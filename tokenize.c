#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 100

char **tokenizeCommands(char *input){
	int BUFFER_SIZE = BLOCK_SIZE, position = 0;
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


char **getCommandDetails(char *input){
	int BUFFER_SIZE = BLOCK_SIZE, position = 0;
	char **tokens = malloc(sizeof(char*) * BUFFER_SIZE);
	char *token;


	tokens[position] = NULL;
	return tokens;
}
