# AOS Assignment 2 
Implement a shell that supports a semi-colon separated list of commands. Use 'strtok' to
tokenize the command. Also, support '&' operator which lets a program run in the background
after printing the process id of the newly created process. Write this code in a modular fashion.
The goal of the assignment is to create a user-defined interactive shell program using
c/cpp that can create and manage new processes. The shell should be able to create a
process out of a system program like emacs, vi, or any user-defined executable. Your
shell can handle background and foreground processes and also input/output
redirections and pipes.
***

## Execution
- By executing following commands in the terminal (LINUX) you can run the program.
- Since there is a makefile created for the program, hence executing only make in the directory in which cpp files are kept is enough to compile them and to execute ./myshell is typed.
- You can type exit or Ctrl+D to exit the shell.
- A single file called headers.h is made to store all headers and function definition.

```shell
make
./myshell
```
***
## Requirement 1
### Problem Statement
- Display requirement:- When you execute your code, a shell prompt of the following form
must appear along with it. Do NOT hard-code the user name and system name here.
_username_@_system_name_:_current_directory_>

- The directory from which the shell is invoked will be the home directory of the shell and
should be indicated by "~".
Your shell should account for random spaces and tabs.
This is NOT hard to implement, just tokenize the input string appropriately.

### Code Explanation and overview
- The code for the display is provide in basicDisplay.cpp and some part of it is present in main.cpp as well.
- Each component like username, systemname nad current directory is fetched using system calls like getenv, gethostname and getcwd and using cout it is formatted to look as asked in the question, and it is put into an infinite loop as it gets printed everytime and input commands can be taken, thus mimicing an actual shell.

***
***



## Requirement 2
### Problem Statement
- cd, echo, pwd:- If the user executes "cd" i.e changes the directory, then the
corresponding change must be reflected in the shell as well. If your current workingdirectory is the directory from which your shell is invoked, then on executing command
"cd .." your shell should display the absolute path of the current directory from the root.
echo and pwd are also shell built-ins. Make sure you implement cd, pwd, and echo.
DON’T use ‘execvp’ or similar commands for implementing these commands.

- For echo, handling multi-line strings and environmental variables is not a
requirement
You do not need to handle escape flags and quotes. You can print the string
as it is. However, you must handle tabs and spaces Example: echo "abc 'ac' abc"
“abc ‘ac’ abc”
For cd apart from the basic functionality, implement the flags “.”, “..”, “-” and “~”.
It is an error for a cd command to have more than one command-line argument
(print: Invalid arguments for error handling). If no argument is present then you
must cd into the home directory.

### Code Explanation and overview
- The code for the same is provided in pwd.cpp, echo.cpp and cd.cpp.
- In main.cpp, the first checking is done if the initial keyword is cd, echo and pwd and then calling the required function. Tokenization is present in a separate file called tokenize.cpp which takes in an input string and using strtok, first tokenizes it on the basis of semicolons and then again tokenizes it on the basis of spaces returning a 2D vector of string as command tokens.
- echo command prints what comes after it, although it is also handled for cases with echo "data" and echo 'data'.
- pwd prints the current working directory for which getcwd is used.
- cd is handled using chdir() and its various flags like cd ., cd .., cd ~ are handled appropriately, although getenv is used sometimes for that. cd - is handled by making a temporary string variable and storing pwd in it, thats how I can get oldpwd.

***
***




## Requirement 3
### Problem Statement
- ls:- Implement the ls command with its two flags “-a” and “-l”. You should be able to
handle all the following cases also:
● ls
● ls -a
● ls -l
● ls .
● ls ..
● ls ~
● ls -a -l
● ls -la / ls -al
● ls <Directory/File_name>
● ls -<flags> <Directory/File_name>
Example: <Name@UBUNTU:~> ls -al test_dir
- For ls, ls -a and ls <Directory_name> outputting the entries in a single column is fine.
● You can assume that the directory name would not contain any spaces.● For the “-l”, kindly, print the exact same content as displayed by your actual Shell. You
can leave out the colors but the content should be the same.
● DON’T use ‘execvp’ or similar commands for implementing this.
● Multiple flags and directory names can be tested. Your shell should also account for
these arguments in any order.
Example:
<Name@UBUNTU:~> ls -l <dir_1> -a <dir_2>
<Name@UBUNTU:~> ls -la <dir_1> <dir_2>All other commands are treated as system
commands like emacs, vi, and so on. The shell must be able to execute them either in
the background or in the foreground.

### Code Explanation and overview
- The code for the same is provided in ls.cpp.
- Different flag combinations of ls are handled by if-else statements and to print differently depending on the flag two boolean variables are maintained to keep track of it, namely aFlag and lFlag.
- To list files in long format, I have used system calls like readdir, opendir, stat and also struct stat object , along with struct passwd and struct group object is also used for printing various permissions and uid and gid.

***
***




## Requirement 4
### Problem Statement
- System commands (background/fg), with and without arguments:-
Foreground processes: For example, executing a "vi" command in the foreground
implies that your shell will wait for this process to complete and regain control when this
process exits.
Background processes: Any command invoked with "&" is treated as a background
command. This implies that your shell will spawn that process and doesn't wait for the
process to exit. It will keep taking other user commands. Whenever a new background
process is started, print the PID of the newly created background process on your shell
also.
Example: <Name@UBUNTU:~> gedit &
456
<Name@UBUNTU:~> ls -l -a

### Code Explanation and overview
- The code for the same is present in main.cpp itself.
- Foreground and background processes are handled using executeCommand function in main.cpp which uses a fork() to create a child process which gets the pid of 0, the child process uses execvp to execute commands while background processes are detected using presence of &, and they are handled separately. The parent waits when a foreground process is created using waitpid, while it resumes for background processes.

***
***





## Requirement 5
### Problem Statement
- Pinfo:-
This prints the process-related info of your shell program. Use of “popen()” for
implementing pinfo is NOT allowed.
Example: <Name@UBUNTU:~> pinfo
pid -- 231Process Status -- {R/S/S+/Z}
memory -- 67854 {Virtual Memory}
Executable Path -- ~/a.out
pinfo <pid> : This prints the process info about the given PID.

### Code Explanation and overview
- The code for the same is present in pinfo.cpp.
- Mainly getrusage is used to obtain the struct rusage object using which is responsible for checking the process status, although pid is handled in main itself using getpid(). /proc helps to compute the executable path and virtual memory is computed using struct rusage object.

***
***





## Requirement 6
### Problem Statement
- Search:- Search for a given file or folder under the current directory recursively. Output should be
True or False depending on whether the file or folder exists
Example: <Name@UBUNTU:~> search xyz.txt
True

### Code Explanation and overview
- The code for the same is provided in search.cpp.
- Search is done recursively and various system calls are used for that including opendir and readdir for opening the current directory. Also, using a struct dirent object, I am checking if it is a directory or a file using its DT_DIR and DT_REG respectively.

***
***










## Requirement 7
### Problem Statement
- I/O redirection:-
Using the symbols <, > and >>, the output of commands, usually written to stdout, can
be redirected to another file, or the input taken from a file other than stdin. Both input
and output redirection can be used simultaneously. Your shell should support this
functionality. Your shell should handle these cases appropriately: An error message
should be displayed if the input file does not exist. The output file should be created
(with permissions 0644) if it does not already exist. In case the output file already
exists, it should be overwritten in case of > and appended to in case of >> .


### Code Explanation and overview
- The code for the same is present in main.cpp only.
- Input and output redirection is handled after input is tokenized, a for loop logic is written to handle more than one redirection operator and dup2 system call is mostly used here as it allocates a new file descriptor that refers to the same open file description and it is attached to stdin and stdout using 0 and 1 respectively. Although initially original standard input and output are saved. If the file doesn't exists for output direction , a new file is being created with permissions 0644.

***
***







## Requirement 8
### Problem Statement
- Pipeline:- A pipe, identified by |, redirects the output of the command on the left as input
to the command on the right. One or more commands can be piped as the following
examples show. Your program must be able to support any number of pipes.


### Code Explanation and overview
- Implementation pending.


***
***






## Requirement 9
### Problem Statement
- Redirection with pipeline:-Input/output redirection can occur within command pipelines,
as the examples below show. Your shell should be able to handle this.


### Code Explanation and overview
- Implementation pending.



***
***




## Requirement 10
### Problem Statement
- Simple signals:-
1. CTRL-Z It should push any currently running foreground job into the background,
and change its state from running to stopped. This should have no effect on the
shell if there is no foreground process running.
2. CTRL-C It should interrupt any currently running foreground job, by sending it the
SIGINT signal. This should have no effect on the shell if there is no foreground
process running.
3. CTRL-D It should log you out of your shell, without having any effect on the
actual terminal.


### Code Explanation and overview
- The implementation of signals is provided in main.cpp itself. Using signal() I am giving SIGINT and SIGTSTP to its signal handling function where using kill I am sending the signals to the foreground process pid on which the required actions are undertaken. For ctrlD, I am just checking if getline returns error for input string.


***
***






## Requirement 11
### Problem Statement
- Autocomplete for all the files/directories under the current directory. Pressing the
TAB key should either complete the command or output a list of matching files/dirs
(space separated) if there are more than one.


### Code Explanation and overview
- Implementation pending.



***
***





## Requirement 12
### Problem Statement
- history:- mplement a ‘history’ command which is similar to the actual history command. The
maximum number of commands it should output is 10. The maximum number ofcommands your shell should store is 20. You must overwrite the oldest commands if
more than 20 commands are entered. You should track the commands across all
sessions and not just one.
- history num: Display only latest num commands.
- Up Arrow Key: On clicking the UP arrow key, you must loop over the previous
commands present in your shell’s history and show them on the prompt. In case you
reach the first command or have no history, then stay on the same command if UP is
pressed.


### Code Explanation and overview
- Implementation done only for part a, ie. history is working and history num is working, up arrow is not working.
- Here history is temporarily stored in a vector string, but permanently stored in a file, which has a relative path in accordance with our program. The file is getting created if it doesn't exist and loadHistory() function is responsible for loading commands from it, if exists or create one if doesn't exists. ifstream and ofstream is used for file manipulation and everytime a command is entered, it is written into the temporary vector and also the file.



***
***
***
















