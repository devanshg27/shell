void runCommand(char *command){
	int BLOCK_SIZE = 100, BUFFER_SIZE = 100, position = 0;
	char **data = malloc(sizeof(char*) * BUFFER_SIZE);
	data = getCommandDetails(command);
}