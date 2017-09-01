#include <tokenize.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **tokenizeCommands(char *input){
	int BUFFER_SIZE = BLOCK_SIZE, position = 0;
	char **tokens = malloc(sizeof(char*) * BUFFER_SIZE);
	if(tokens == NULL){
		perror("Malloc Failed\n");
		exit(0);
	}

	char *token;

	token = strtok(input, ";");
	while(token != NULL){
		tokens[position] = token;
		++position;
		if(position == BUFFER_SIZE){
			BUFFER_SIZE += BLOCK_SIZE;
			tokens = realloc(tokens, BUFFER_SIZE * sizeof(char*));
			if(tokens == NULL){
				perror("Realloc Failed\n");
				exit(0);
			}
		}
		token = strtok(NULL, ";");
	}
	tokens[position] = NULL;
	return tokens;
}
