#include "../shell.h"
#include "commands.h"

// repeat command
int repeat(char* words[] , int word_count){
    // printf("\nin repeat : ");
    if(word_count < 3){
        printf("repeat : argument error");
        return 1;
    }
    int i = atoi(words[1]);
    for(int j = 0; j < i; j++){
        runcommand(words + 2, word_count -2);
    }

    return 0;
}