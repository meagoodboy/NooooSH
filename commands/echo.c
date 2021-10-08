#include "../shell.h"
#include "commands.h"

// echo command
int echo(char* words[] , int word_count){
    int index = 1;
    while(index < word_count)
    {
        printf("%s ",words[index]);
        index++;
    }
    printf("\n");
    return 0;
}