# Shell

The goal of the project is to create a user defined interactive shell program that can
create and manage new processes. The shell should be able to create a process out of a
system program like emacs, vi or any user-defined executable.

### Features!

  - Multiple semi - colon seperated commands are supported.
  - Support for all built in commands along with any arguments/ flags that the command accepts.
  - Implemented commands like **cd, pwd, echo, ls, pinfo and nightswatch** ls supports flags in the form -l, -a, -al, -la and woks for both files and directories and accepts multiple arguments too.
  - Both foreground and background processes are supported.
  - Colour support in ls command as well as the prompt.
  - Makefile is also added to make running and linking easy for the user.
  - Support for **~ : Home** like in bash.  

### Implementation

The code is written in modular style and easily is extensible. There are seperate files for functions with seperate tasks.

  - **shell.c** : The main file which initialises variables and controls the main flow.
  - **input.c** : Accepts input from the user and calls the tokenize function to split it into multiple commands.
  - **tokenize.c** : Tokenizes a given input string into multiple commands.
  - **execute.c** : The main function is the _runCommand_ function which executes a command given to it, depenending on whether it is a background process, self-implemented command or otherwise.
  - There are seperate files for self-implemented commands that run a particular command.

### Usage

```sh
$ make
$ ./shell
$ make clean
```