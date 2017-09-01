#include <lsbuiltin.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

int checkFile(char *fileName){
	struct stat data;
	int val = stat(fileName, &data);
	if(val == -1){
		perror("Stat Error");
		exit(0);
	}
	else if(S_ISDIR(data.st_mode) == 0) return 1;
	else return 0;
}

void printFileName(char *dirName, char *fileName){
	struct stat data;
	char fullFilePath[PATH_MAX + 1];
	strcpy(fullFilePath, dirName);
	strcat(fullFilePath, "/");
	strcat(fullFilePath, fileName);
	int val = stat(fullFilePath, &data);
	if(val == -1){
		perror("Stat Error");
		exit(0);
	}
	else if(S_ISDIR(data.st_mode)) printf("\033[1;34m");
	else if(S_ISCHR(data.st_mode)) printf("\033[40;33;1m");
	else if(S_ISLNK(data.st_mode)) printf("\033[01;36m");
	else if(S_ISFIFO(data.st_mode)) printf("\033[40;33m");
	else if(S_ISSOCK(data.st_mode)) printf("\033[01;35m");
	else if(S_ISBLK(data.st_mode)) printf("\033[40;33;1m");
	else if(data.st_mode & S_IXUSR) printf("\033[32;1m");
	printf("%s\033[0m", fileName);
}

void display(struct stat data){
	if(S_ISDIR(data.st_mode)) printf("d");
	else if(S_ISCHR(data.st_mode)) printf("c");
	else if(S_ISLNK(data.st_mode)) printf("l");
	else if(S_ISFIFO(data.st_mode)) printf("p");
	else if(S_ISSOCK(data.st_mode)) printf("s");
	else if(S_ISBLK(data.st_mode)) printf("b");
	else printf("-");
	printf("%s", (data.st_mode & S_IRUSR) ? "r" : "-");
	printf("%s", (data.st_mode & S_IWUSR) ? "w" : "-");
	printf("%s", (data.st_mode & S_IXUSR) ? "x" : "-");
	printf("%s", (data.st_mode & S_IRGRP) ? "r" : "-");
	printf("%s", (data.st_mode & S_IWGRP) ? "w" : "-");
	printf("%s", (data.st_mode & S_IXGRP) ? "x" : "-");
	printf("%s", (data.st_mode & S_IROTH) ? "r" : "-");
	printf("%s", (data.st_mode & S_IWOTH) ? "w" : "-");
	printf("%s", (data.st_mode & S_IXOTH) ? "x" : "-");
	printf(" ");
	printf("%3lld ", (long long)data.st_nlink);
	printf("%10s ", getpwuid(data.st_uid)->pw_name);
	printf("%10s ", getgrgid(data.st_gid)->gr_name);
	printf("%10lld ", (long long)data.st_size);

	char *c_time_string = ctime(&data.st_mtim.tv_sec);
	c_time_string[strlen(c_time_string) - 1] = '\0';
	printf("%s ", c_time_string);
}

void displayFile(char *fileName, int l, int a){
	struct stat data; 
	if(l){
		if(stat(fileName, &data) != -1){
			display(data);
		}
		else{
			perror("Stat Error");
			exit(0);
		}
	}
	printFileName(".", fileName);
	printf("\n");
}

void listFileInfo(char *directoryName, char *fileName){
	struct stat data;
	char filePath[PATH_MAX + 1];
	if(strcpy(filePath, directoryName) != NULL){
		if(strcat(filePath, "/") != NULL){
			if(strcat(filePath, fileName) != NULL){
				int val = stat(filePath, &data);
				if(val == -1){
					perror("Stat Error");
					exit(0);
				}
				display(data);
			}
			else{
				perror("Strcat Error");
				exit(0);
			}
		}
		else{
			perror("Strcat Error");
			exit(0);
		}
	}
	else{
		perror("Strcpy Error");
		exit(0);
	}
}

void show(char *x, int l, int a){

	DIR* directory;
	struct dirent *current;

	directory = opendir(x);
	if(directory == NULL){
		perror("Opendir Error");
		exit(0);
	}

	current = readdir(directory);
	
	for(; current != NULL; current = readdir(directory)){
		if((current->d_name)[0] == '.'){
			if(a){
				if(l){
					listFileInfo(x, current->d_name);
				}
				printFileName(x, current->d_name);
				printf("\n");
			} 
		}
		else{
			if(l){
				listFileInfo(x, current->d_name);
			}
			printFileName(x, current->d_name);
			printf("\n");
		} 
	}

	closedir(directory);
}

void listData(char *argument, int L, int A, char *home_directory) {
	char path[PATH_MAX + 1];
	if(strcpy(path, argument) == NULL) {
		perror("Strcpy Error");
		exit(0);
	}
	if(strcmp(argument, "~") == 0) {
		if(strcpy(path, home_directory) == NULL) {
			perror("Strcpy Error");
			exit(0);
		}
	}
	else if(argument[0] == '~' && argument[1] == '/') {
		if(strcpy(path, home_directory) == NULL) {
			perror("Strcpy Error");
			exit(0);
		}
		if(strcat(path, &argument[1]) == NULL) {
			perror("Strcat Error");
			exit(0);
		}
	}
	int val = checkFile(path);
	if(val) displayFile(path, L, A);
	else show(path, L, A);
}

void ls(char **arguments, int count, char *home_directory){
	int A = 0, L = 0, idx = 1;

	char *location = malloc(sizeof(char) * PATH_MAX);
	if((location) == NULL){
		perror("Malloc Failed");
		exit(0);
	}

	while(arguments[idx] != NULL){
		if(arguments[idx][0] == '-' && strlen(arguments[idx]) > 1){
			for(int j=1; j<strlen(arguments[idx]); ++j){
				if(arguments[idx][j] == 'a') A = 1;
				else if(arguments[idx][j] == 'l') L = 1;
				else{
					printf("Invalid Flag");
					return;
				}
			}
		}
		++idx;
	}

	idx = 1, count = 0;
	while(arguments[idx] != NULL){
		if(arguments[idx][0] == '-'){
			if((int)strlen(arguments[idx]) > 1);
			else ++count;
		}
		else ++count;
		++idx;
	}

	int yes = 0, pos = 0; idx = 1;
	while(arguments[idx] != NULL){
		if(arguments[idx][0] == '-'){
			if((int)strlen(arguments[idx]) > 1);
			else{
				++pos;
				if(count > 1) printf("%s:\n", arguments[idx]);
				listData(arguments[idx], L, A, home_directory);
				yes = 1;
				if(pos != count) printf("\n");
			}
		}
		else{
			++pos;
			if(count > 1) printf("%s:\n", arguments[idx]);
			listData(arguments[idx], L, A, home_directory);
			yes = 1;			
			if(pos != count) printf("\n");	
		}
		++idx;
	}

	if(yes == 0){

		char *current = malloc(sizeof(char) * 2);
		if(current == NULL){
			perror("Malloc Failed");
			exit(0);
		}

		current = ".";

		int val = checkFile(current);

		if(val) displayFile(current, L, A);
		else show(current, L, A);
	}
}