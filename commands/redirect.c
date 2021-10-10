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

    int inp_loc = 0, out_flag = 0;
    int file_in, file_out;
    int in_flag = 0;
    char* wordscopy[INP_SIZE];
    for(int i = 0 ; i < word_count ; i++){
        if(words[i][0] == '<'){
            file_in = open(words[i+1], O_RDONLY, 0644);
            if(file_in < 0){
                perror("redirect ");
            }
            dup2(file_in, STDIN_FILENO);
            if(inp_loc == 0) inp_loc = i;
            in_flag = 1;
        }
        else if(strlen(words[i]) == 2){
            // fprintf(stderr,"in double ");
            if(words[i][0] == '>' && words[i][1] == '>'){
            // fprintf(stderr,"in double 2");
            file_out = open(words[i+1], O_CREAT | O_APPEND | O_WRONLY, 0644);
            if(file_out < 0){
                perror("redirect ");
            }
            dup2(file_out, STDOUT_FILENO);
            if(inp_loc == 0) inp_loc = i;
            out_flag = 1;
        }}
        else if(words[i][0] == '>'){
            file_out = open(words[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if(file_out < 0){
                perror("redirect ");
            }
            dup2(file_out, STDOUT_FILENO);
            if(inp_loc == 0) inp_loc = i;
            out_flag = 1;
        }


    }
    for(int i = 0; i < inp_loc ; i++){
        wordscopy[i] = words[i];
    }
    wordscopy[inp_loc] = NULL;
    // fprintf(stderr, "lenght  = %d\n", inp_loc);
    runcommand_f(wordscopy, inp_loc);

    if(in_flag == 1){
        close(file_in);
    }
    if(out_flag == 1){
        close(file_out);
    }
    return 0;
}