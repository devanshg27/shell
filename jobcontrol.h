void initJobControl();
int printJobs(char **arguments, int count, char *home_directory);
int setenvBuiltin(char **arguments, int count, char *home_directory);
int getenvBuiltin(char **arguments, int count, char *home_directory);
int unsetenvBuiltin(char **arguments, int count, char *home_directory);
int overkill(char **arguments, int count, char *home_directory);
void addToBackground(pid_t PID, char* commandArgument);
int quitBuiltin(char **arguments, int count, char *home_directory);