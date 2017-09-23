void initJobControl();
int printJobs(char **arguments, int count, char *home_directory);
int setenvbuiltin(char **arguments, int count, char *home_directory);
int getenvbuiltin(char **arguments, int count, char *home_directory);
int unsetenvbuiltin(char **arguments, int count, char *home_directory);
int overkill(char **arguments, int count, char *home_directory);
void addToBackground(pid_t PID, char* commandArgument);