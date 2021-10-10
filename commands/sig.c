#include "../shell.h"
#include "commands.h"

int sig(char* words[] , int word_count){
    
    if(word_count != 3){
        printf("\nsig : argument error");
        return 1;
    }
    int pid = atoi(words[1]);
    int found = 0;

    if(pid > total_children_bg + 1 || pid < 1){
            found = -1;
    }
    pid --;

    if (found == -1){
        printf("\n fg : no child in bg with given pid");
        return 1;
    }

    int ret = kill(cp[pid].child_id, atoi(words[2]));

    if(ret < 0){
        perror("sig");
        return 1;
    }
    return 0;

}