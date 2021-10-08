#include "../shell.h"
#include "commands.h"

int fg_p(char* words[] , int word_count){
    
    if(word_count != 2){
        printf("\nfg : argument error");
        return 1;
    }

    int status_w;

    pid_t pid = atoi(words[1]);
    int found = 0;

    for(int i = 0; i < total_children_bg ; i ++){
        if(pid == cp[i].child_id){
            found = i;
            break;
        }
    }

    if (found == 0){
        printf("\n fg : no child in bg with given pid");
        return 1;
    }
    else{
        removechild(pid);
        int ret = kill(pid, SIGCONT);
        if(ret < 0){
            perror("fg:");
        }
        fgpi.child_id = pid;
        strcpy(fgpi.childname ,cp[found].childname);
        waitpid(pid, &status_w ,WUNTRACED);
    }

    return 0;

}