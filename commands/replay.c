#include "../shell.h"
#include "commands.h"

int replay(char* words[] , int word_count)
{
    char* command_part[INP_SIZE] ; 
    int comm_size = 0;
    int comm_index = -1;
    int period = -1;
    int interval = -1;

    for(int i = 1 ; i < word_count ; i++){
        if(words[i][0] == '-'){
            if(strcmp(words[i], "-command") == 0){
                comm_index = i;
                for(int j = i+1 ; j < word_count ; j++){

                    // fprintf(stderr,"here words  : %s \n", words[j]);
                    
                    if(strcmp(words[j], "-interval") == 0 ){
                        // fprintf(stderr,"here \n");
                        break;
                    }
                    else if(strcmp(words[j], "-period") == 0){
                        // fprintf(stderr,"here 2\n");
                        break;
                    }
                    else{
                        command_part[comm_size] = words[j];
                        comm_size ++;

                    }
                }
            }
            else if(strcmp(words[i], "-interval") == 0){
                interval = atoi(words[i+1]);
            }
            else if(strcmp(words[i], "-period") == 0){
                period = atoi(words[i+1]);
            }
        }
    }
    // fprintf(stderr, "period : %d   interval : %d,  command_size :  %d \n", period, interval , comm_size);
    command_part[comm_size] = NULL;
    // for (int k = 0; k <comm_size ; k++){
    //     fprintf(stderr, "%s\n", command_part[k]);
    // }
    
    if(interval < 0){
        printf("\nreplay : invalid interval");
        return 1;
    }
    if(period < 0){
        printf("\nreplay : invalid period");
        return 1;
    }
    if(comm_size <= 0){
        printf("\nreplay : no command given");
        return 1;
    }

    int k = period/interval;
    for (int j = 0; j< k; j++){
        sleep(interval);
        runcommand_f(command_part, comm_size);
        
    }

    
}