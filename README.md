# Shell

The goal of the project is to create a user defined interactive shell program that can
create and manage new processes. The shell should be able to create a process out of a
system program like emacs, vi or any user-defined executable.

### Features!

  - Multiple semi - colon seperated commands are supported.
  - Added **autocompletion** and **history** support forr commands.
  - Support for all built in commands along with any arguments/ flags that the command accepts.
  - Implemented commands like **cd, pwd, echo, ls, pinfo and nightswatch** ls supports flags in the form -l, -a, -al, -la and woks for both files and directories and accepts multiple arguments too.
  - Both foreground and background processes are supported.
  - Colour support in ls command as well as the prompt.
  - Makefile is also added to make running and linking easy for the user.
  - Support for **~ : Home** like in bash.
  - Implemented input and output redirection functionality with support for append mode too.
  - Implemented command redirection using **piping('|' symbol)** with support for any number of pipes.
  - Piping and redirection can be used simultaneously.
  - Support for environment variables using **setenv**, **getenv** and **unsetenv**.
  - Implemented job control and support for commands like:
    - **jobs** to view all jobs
    - **overkill** to kill all jobs
    - **kjob** to send signal to a job
    - **fg** to bring a background job to foreground
    - **bg** to resume a stopped background job
  - Implemented **quit** command whcih kills all the jobs and then exits the shell.
  - Support for signals like CTRL-D, CTRL-C, SIGINT, SIGCHLD etc.

### Implementation

The code is written in modular style and easily is extensible. There are seperate files for functions with seperate tasks.

  - **shell.c** : The main file which initialises variables and controls the main flow.
  - **input.c** : Accepts input from the user and calls the tokenize function to split it into multiple commands.
  - **tokenize.c** : Tokenizes a given input string into multiple commands.
  - **execute.c** : The main function is the _runCommand_ function which executes a pipeline given to it along with handling whether it has to be executed in the foreground or background and it also handles the redirection or piping.
  - **jobcontrol.c**  :  This file has signal handlers for SIGCHLD, SIGINT, SIGTSTP, SIGTTOU and has all the functions related to job control like jobs, overkill, kjob, fg and bg.
  - There are seperate files for self-implemented commands that run a particular command.

### Usage

```sh
$ make
$ ./shell
$ make clean
```