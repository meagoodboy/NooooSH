# __NooooSH__
#### _A simple shell created using C_

NooooSH is a simple shell made using C with low level Linux system calls
for OSN Course Monsoon 2021 . Some of its basic functionalities are
- Common commands like cd,ls ...
- Piping and Redirection
- Shell specific commands (will be mentioned in detail later)

## Downloading and Running
To download and run the shell , 
```sh
git clone https://github.com/meagoodboy/NooooSH.git
cd NooooSH
make
./noooosh
```
or if you have already downloaded the folder containing the shell files, go 
to that folder ,
```sh
make noooosh
./noooosh
```
if the shell is not running or ,if the history is not being initialised
on running the shell, it is recommended to remove the old history file 
present in var/tmp and also the old excecutable of the shell
```sh
rm -rf noooosh /var/tmp/shell_history.txt 
```
## Commands

These are the few inbuilt commands in the NooooSH:

- ls [-l -a] - to display the files and folders.
- cd - to change the current working directory.
- echo - To print text on the shell.
- pwd - to print the current working directory
- history - to print the previous commands used
- jobs [-r -s] - to print the active background jobs in the shell
- repeat - repeats a command 
- replay [-command -period -interval] - runs a command after
very interval until the given period
- fg - brings a background process to foreground given the process id
- bg - changes the state of the background process to running given the
process id
- sig - sends the given signal to the given process id
- pinfo - prints the information of the process given the process id

## Other features
- Piping
- Redirection
- Running of background process and foreground process (inbuilt and
other linux commands)
- signal handling for ctrl+c, ctrl+z, ctrl+d

## File structure
The main runninng code of the shell is present in the file shell.c. And all the header
files , global variables , global functions are defined in the header file shell.h.
Each basic command has a file named <command>.c where the functions needed for
running the command is stored. All the functions present in the commands folder 
is defined in the header file called commands.h. and its is imported to shell.c for
the usage
