#include "../shell.h"
#include "commands.h"

int bg_p(char* words[] , int word_count){
    
    if(word_count != 2){
        printf("\nbg : argument error");
        return 1;
    }

    pid_t pid = atoi(words[1]);
    int found = 0;

    for(int i = 0; i < total_children_bg ; i ++){
        if(pid == cp[i].child_id){
            found = 1;
        }
    }

    if (found == 0){
        printf("\n fg : no child in bg with given pid");
        return 1;
    }

    int ret = kill(atoi(words[1]), SIGCONT);

    if(ret < 0){
        perror("bg:");
        return 1;
    }
    return 0;

}