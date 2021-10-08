#include "../shell.h"
#include "commands.h"

int replay(char* words[] , int word_count)
{
    char* command_part[100] ; 
    for (int k = 0 ; k <100 ; k++){
        command_part[k] = (char *)malloc(INP_SIZE);
    }
    int comm_size = 0;
    int comm_index = -1;
    int period = -1;
    int interval = -1;

    for(int i = 1 ; i < word_count ; i++){
        if(words[i][0] == '-'){
            if(strcmp(words[i], "-command") == 0){
                comm_index = i;
                for(int j = i+1 ; j < word_count ; j++){
                    if(strcmp(words[i], "-interval") == 0 || strcmp(words[i], "-period") == 0){
                        break;
                    }
                    else{
                        strcpy(command_part[comm_size], words[j]);
                        comm_size ++;

                    }
                }
            }
            else if(strcmp(words[i], "-interval") == 0){
                interval = atoi(words[i+1]);
            }
            else if(strcmp(words[i], "-interval") == 0){
                period = atoi(words[i+1]);
            }
        }
    }

    
}