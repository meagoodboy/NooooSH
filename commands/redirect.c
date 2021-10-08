#include "../shell.h"
#include "commands.h"

int check_redirection(char* words[] , int word_count){
    for (int i = 0; i < word_count ; i ++){
        for (int j= 0; j < strlen(words[i]); j ++){
            if ( words[i][j] == '>' || words[i][j] == '<'){
                return 1;
            }
        }
    }
    return 0;
}

int manage_redirection(char* words[] , int word_count){
    return 0;
}