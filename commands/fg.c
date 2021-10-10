#include "../shell.h"
#include "commands.h"

int fg_p(char* words[] , int word_count){
    
    if(word_count != 2){
        printf("\nfg : argument error");
        return 1;
    }

    int status_w;

    int pid = atoi(words[1]);
    int found = 0;

    if(pid > total_children_bg + 1 || pid < 1){
            found = -1;
    }
    pid --;
    // fprintf(stderr, "pid here is : %d , child id is : %d", pid, cp[pid].child_id);
    if (found == -1){
        printf("\n fg : no child in bg with given pid");
        return 1;
    }
    else{
        // removechild(cp[pid].child_id);
        // int ret = kill(cp[pid].child_id, SIGCONT);
        // if(ret < 0){
        //     perror("fg:");
        // }
        // fgpi.child_id = cp[pid].child_id;
        // strcpy(fgpi.childname ,cp[pid].childname);
        
        
//  terminal control 

        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);

        fgpi.child_id = cp[pid].child_id;
        strcpy(fgpi.childname , cp[pid].childname);

        if(tcsetpgrp(STDIN_FILENO, getpgid(cp[pid].child_id)))
        {
            fprintf(stderr, "Could not give terminal control to job %d\n", pid);
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);

            return -1;
        }
        if(kill(fgpi.child_id, SIGCONT))
        {
            fprintf(stderr, "Unable to resume job %d\n", pid);
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            return -1;
        }

        if(waitpid(fgpi.child_id, &status_w ,WUNTRACED) > 0){
            removechild(fgpi.child_id);
            if(WIFSTOPPED(status_w))
                addchild(fgpi.child_id, fgpi.childname);
        }
        if(tcsetpgrp(STDIN_FILENO, getpgid(0)))
        {
            fprintf(stderr, "Could not return terminal controll to the shell. Exitting the shell\n");
        }
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);

    }

    return 0;

}