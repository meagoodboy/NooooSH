#include "../shell.h"
#include "commands.h"


// run foreground
int fg(char* words[] , int word_count){
    
    int pid = fork();
    int status_w;
    if( pid < 0){
        perror("\nforking ");
        return pid;
    }
    // printf("\n fg  : %s", words[0]);
    if(pid == 0){
        int status = execvp(words[0], words);
        if(status < 0){
            perror("\nexecvp ");
            return 1;
        }

    }else{
        fgpi.child_id = pid;
        strcpy(fgpi.childname ,words[0]);
        waitpid(pid, &status_w ,WUNTRACED);
    }
    return 0;
}
