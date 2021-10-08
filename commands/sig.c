#include "../shell.h"
#include "commands.h"

int sig(char* words[] , int word_count){
    
    if(word_count != 3){
        printf("\nsig : argument error");
        return 1;
    }

    int ret = kill(atoi(words[1]), atoi(words[2]));

    if(ret < 0){
        perror("sig");
        return 1;
    }
    return 0;

}