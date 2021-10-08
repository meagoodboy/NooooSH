#include "../shell.h"
#include "commands.h"

int jobs(char* words[] , int word_count){
    
    if(word_count > 2){
        printf("jobs : argument error");
        return 1;
    }

    int s_flag = 0;
    int r_flag = 0;
    int indices[100];
    for (int j=0;j<100;j++)
    {
        indices[j] = j;
    }

    if(word_count == 2){
        if(words[1][0] == '-')
        {
            if(words[1][1] == 's'){
                s_flag = 1;
            }
            else if(words[1][1] == 'r'){
                r_flag = 1;
            }
            else{
                printf("\njobs: invalid flag");
            }
        }
        else{
            printf("\njobs: invalid argument");
        }
    }

    int statuses[100] = {0};

    for(int i = 0; i < total_children_bg - 1; i++){
        for(int j = i + 1; j < total_children_bg; j++){
            int x = strcmp(cp[indices[i]].childname, cp[indices[j]].childname );
            if(x > 0){
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    // for(int i = 0; i < total_children_bg; i++){
    //     printf("\n index chk  :  %d", indices[i]);
    //     }


    for(int i = 0; i < total_children_bg; i++){
        // printf("\nlol : %d", indices[i]);
        
        pid_t pid = cp[indices[i]].child_id;

        char* proc_path = (char *) malloc (100);
        sprintf(proc_path,"/proc/%d/stat", pid);
        FILE* process_info;
        process_info = fopen(proc_path, "r");
        char* full_proc_info = (char *) malloc (INP_SIZE);
        // printf("\nlol 1 : %d", indices[i]);

        fread(full_proc_info, INP_SIZE, 1, process_info);


        char* file_stats[100] = {NULL}; 
        int k=0;
        file_stats[k] = strtok(full_proc_info, " ");
        while(file_stats[k]){
            file_stats[++k] = strtok(NULL, " ");
        }

        // printf("\nlol 2 : %d", indices[i]);

        if(file_stats[2][0] == 'S') {
            statuses[i] = 1;
            cp[indices[i]].status = 1;
        } 
        fclose(process_info);
        free(proc_path);
        // printf("\nlol 3 : %d", indices[i]);

        // printf("\n in jobs: %s  %d %d",cp[indices[i]].childname,cp[indices[i]].child_id, indices[i] );

        if (r_flag == 0 && s_flag == 0){
            printf("\n%s %s [%d]",(cp[indices[i]].status == 1)?"Running":"Stopped", cp[indices[i]].childname , cp[indices[i]].child_id );
        }
        else if(r_flag == 1){
            if(cp[indices[i]].status == 1){
                printf("\n%s %s [%d]",(cp[indices[i]].status == 1)?"Running":"Stopped", cp[indices[i]].childname , cp[indices[i]].child_id );
            }
        }
        else if(s_flag == 1){
            if(cp[indices[i]].status == 0){
                printf("\n%s %s [%d]",(cp[indices[i]].status == 1)?"Running":"Stopped", cp[indices[i]].childname , cp[indices[i]].child_id );
            }
        }
    }

    // for(int i = 0; i < total_children_bg; i++){
    //     printf("\nlol : %d", indices[i]);
    // }


    return 0;
}