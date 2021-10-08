#include "../shell.h"
#include "commands.h"


int handlepipes(char* input_line , int pipe_count){
    // printf("\n in pipes beg");
    char* piped_words_in_line[INP_SIZE];
            
    for(int j = 0; j < INP_SIZE; j++){
        piped_words_in_line[j] = NULL;
    }
    // printf("\n in pipes beg 2");
    char* pipe_pointer = strtok(input_line, "|");
    int count = 0;
    // printf("\n in pipes beg 3");
    int words_piped;
    for ( int i = 0; pipe_pointer ; i++ ){
        piped_words_in_line[i] = pipe_pointer;
        pipe_pointer = strtok(NULL, "|");
        count ++;
    }
    // printf("\n in pipes beg 4");
    if(count - 1 != pipe_count){
        printf("\npipr : error");
    }

    int fds[200];
    // printf("\n in pipes beg 5: %d %d", pipe_count, count);
    for (int i = 0; i < 2 * pipe_count; i += 2){
        // printf("\nhi");
        if (pipe(fds + i))
        {
            printf("pipes : Unable to create pipe\n");
        }   
        // printf("\nhi 2");

    }

    // printf("\n in pipes");
    fflush(stdout);
    int std_bkp = dup(STDOUT_FILENO);
    int stdin_bkp = dup(STDIN_FILENO);
    for(int j = 0; j < count; j++){

        // printf("\n in pipes loop");
        fflush(stdout);
        if( j < pipe_count){
            dup2(fds[2*j + 1], STDOUT_FILENO);
        }else{
            dup2(std_bkp, STDOUT_FILENO);
        }
        if (j > 0){
            // close(fds[2*j + 1]);
            dup2(fds[2*j - 2], STDIN_FILENO);
        }
        // printf("\n in pipes loop before exec bef");
        // execute
        // printf("\n in pipes loop before exec");
        char* words_in_line[INP_SIZE];
        for(int z = 0; z < INP_SIZE; z++){
            words_in_line[z] = NULL;
        }
        char* word_pointer = strtok(piped_words_in_line[j], " \t\n");
        int words;
        for ( words = 0; word_pointer ; words++ ){
            words_in_line[words] = word_pointer;
            word_pointer = strtok(NULL, " \t\n");
        }
        int x = runcommand(words_in_line , words);
        if( j < pipe_count){
            close(fds[2*j + 1]);
        }else{
            close(std_bkp);
        }
        if (j > 0){
            // close(fds[2*j + 1]);
            close(fds[2*j - 2]);
        }
    }
    dup2(stdin_bkp, STDIN_FILENO);
    close(stdin_bkp);
    fflush(stdout);
    // for (int k = 0; k < 2 * pipe_count; k++){
    //     close(fds[k]);
    // }
    return 0;
}