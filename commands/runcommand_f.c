#include "../shell.h"
#include "commands.h"

int runcommand_f(char* words[] , int word_count){
    int rval = 0;
    if(strcmp(words[0], "cd") == 0){
        rval = cd(words, word_count);
    }
    else if(strcmp(words[0], "exit") == 0){
        addHistory(words, word_count);
        exit(0);
    }
    else if(strcmp(words[0], "ls") == 0){
        rval = ls(words, word_count);
    }
    else if(strcmp(words[0], "pwd") == 0){
        rval = pwd(words, word_count);
    }
    else if(strcmp(words[0], "echo") == 0){
        rval = echo(words, word_count);
    }
    else if(strcmp(words[0], "repeat") == 0){
        rval = repeat(words, word_count);
    }
    else if(strcmp(words[0], "pinfo") == 0){
        rval = pinfo(words, word_count);
    }
    else if(strcmp(words[0], "history") == 0){
        rval = getHistory(words, word_count);
    }
    else if(strcmp(words[0], "jobs") == 0){
        rval = jobs(words, word_count);
    }
    else if(strcmp(words[0], "sig") == 0){
        rval = sig(words, word_count);
    }
    else if(strcmp(words[0], "replay") == 0){
        rval = replay(words, word_count);
    }
    else if(strcmp(words[0], "fg") == 0){
        rval = fg_p(words, word_count);
    }
    else if(strcmp(words[0], "bg") == 0){
        rval = bg_p(words, word_count);
    }
    else if(strcmp(words[word_count - 1] + strlen(words[word_count - 1]) - 1, "&" ) == 0){
        rval = bg(words, word_count);
    }
    else{
        rval = fg(words, word_count);
    }
    

    return rval;
}