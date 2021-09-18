# NooooSH 

## How to run

run the command 
    make noooosh
to create an executable file
and reun
    ./nooosh to run the shell

## Basic info

all the inbuilt function in the shell has their own function in the code except for the commands run by execvp i.e he foreground and background process.
cerian global variables are defined to keep track of HOME, PWD etc ..

when the shell starts an initialise function initialises the global variables and he control is given to an infinite loop which displays the prompt, the getInput function waits for an input and tthe input is passed to manageinput which convets the input to tokens for easier usage, the runcommand recogonises the function and passes control to the nesessary functions if needed and the command is saved in history. background process ending signal is managed by sighandler function which prints appropriae info about the completed background process. The history is saved in the var/tmp/location/

cntrl+d kills all the child process and kills the terminal

## inbuilt commands

ls
cd
echo
pwd
pinfo
history
exit
