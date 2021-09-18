#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

#define clear printf("\033[H\033[J")
#define red() printf("\033[0;31m")
#define yellow() printf("\033[0;33m")
#define green() printf("\033[0;31m")
#define blue() printf("\033[0;34m")
#define purple() printf("\033[0;35m")
#define cyan() printf("\033[0;36m")
#define reset_color() printf("\033[0m")
#define INP_SIZE 4096

int total_children_bg = 0;
int history_count = 0;
char* history_list[20] = {NULL};
int children_pid[1000];
struct child_process{
    char childname[1000];
    int child_id;
}cp[100];
extern int errno;
char USER[100];
char HOST[100];
char HOME[100];
char PWD[1000];
char LWD[1000];


int runcommand(char* words[] , int word_count);

int initHistory(){
    //malloc hisory
    for(int i = 0; i< 20 ; i++){
        history_list[i] = (char *)malloc(INP_SIZE);
    }
    char * temp_history = (char *)malloc(INP_SIZE);
    char * history_pointer;
    FILE *history_loc;
    history_loc = fopen("/var/tmp/shell_history.txt", "r");
    if(history_loc == NULL){
        history_count = 0;
    }
    else{
        history_count = 0;
        fread(temp_history, 1, 20 * INP_SIZE, history_loc);
        history_pointer = strtok(temp_history,"\n");
        while(history_pointer){
            strcpy(history_list[history_count], history_pointer);
            history_count ++;
            history_pointer = strtok(NULL,"\n");
        }
        fclose(history_loc);
    }
    free(temp_history);
    printf("\nHistory iniitalised sucessfullly");
    return 0;
}

int closeHistory(){
    
    char* all_history = (char*) malloc (20 * INP_SIZE);
    for(int i = 0 ;i < history_count; i++){
        strcat(all_history, history_list[i]);
        strcat(all_history, "\n");
    }
    FILE *history_loc;
    history_loc = fopen("/var/tmp/shell_history.txt", "w");
    fwrite(all_history, 1, strlen(all_history) , history_loc);
    fclose(history_loc);
    free(all_history);
    for(int i = 0; i< 20 ; i++){
        free(history_list[i]);
    }
    return 0;
}

int addHistory(char* words[], int word_count){
    if(history_count < 20){ 
        strcpy(history_list[history_count],words[0]);
        for(int i = 1 ; i < word_count ; i++){
            strcat(history_list[history_count], " ");
            strcat(history_list[history_count], words[i]);

        }
        history_count ++;
    }else{
        for(int i = 0 ; i < 19 ; i ++){
            strcpy(history_list[i],history_list[i+1]);
        }
        strcpy(history_list[19],words[0]);
        for(int i = 1 ; i < word_count ; i++){
            strcat(history_list[19], " ");
            strcat(history_list[19], words[i]);

        }
    }
    return 0;
}

int getHistory(char* words[], int word_count){
    if(word_count > 2){
        printf("\nhistory : invalid no of arguments");
        return 1;
    }
    else if(word_count == 1){
        for (int i = 0 ; i < history_count ; i++){
            printf("\n%s", history_list[i]);
        }
    }
    else{
        if(atoi(words[1]) > history_count){
            printf("\nhistory : invalid history no");
            return 1;
        }
        for (int i = history_count - atoi(words[1]) ; i < history_count ; i++){
            printf("\n%s", history_list[i]);
        }
    }
    return 0;
}



// function to initialise basic environment variables 
int initialise(){
    clear;
    purple();
    printf("\n ************************************************************ \n");
    printf("   WELCOME TO A REGULAR SHELL EXCLUSIVELY FOR OSN PURPOSES");
    printf("\n ************************************************************ \n\n");
    reset_color();
    initHistory();
    strcpy(USER, getenv("USER"));
    strcpy(HOME, getenv("HOME"));
    getcwd(PWD, sizeof(PWD));
    strcpy(LWD, PWD);
    gethostname(HOST, sizeof(HOST));
    return errno;
}

// function to create the PWD path
char* formatpath(){
    char* pwd =  (char*) malloc (INP_SIZE);
    strcpy(pwd , PWD);
    char pwd_ini_string[INP_SIZE];
    char* output_pwd = (char*) malloc (INP_SIZE);
    output_pwd[0] = '~';
    // strncpy(pwd_ini_string, pwd , strlen(HOME));
    int i;
    for(i = 0; i< strlen(HOME);i ++){
        pwd_ini_string[i] = pwd[i];
    }
    pwd_ini_string[i] = '\0';
    // printf("\nsou : %s    %s", pwd_ini_string , pwd);
    if(strcmp(HOME, pwd_ini_string) == 0){
        strcat(output_pwd , pwd + strlen(HOME) );
        // printf("\noutput1 : %s",pwd + strlen(HOME) );
    }
    else{
        strcpy(output_pwd, pwd);
        // printf("\noutput2 : %s", output_pwd);
    }
    return output_pwd;
}

// function to print the command prompt
int getPWD(){
    getcwd(PWD, sizeof(PWD));
    char* PWD_out = formatpath();
    // printf("\n   %s       %s   %s", PWD , HOME , PWD_out);
    printf("\n\033[0;32m<%s@%s:\033[0;34m%s\033[0;32m>\033[0m ", USER, HOST, PWD_out);
    return errno;
}

// function to format input to the terminal
int getINPUT(char* input_str){


    char* buffer = NULL;     
    size_t bufr_size_m = 0;                           
    getline(&buffer, &bufr_size_m, stdin);
    // printf("%s", buffer);
    // perror("lol");
    strcpy(input_str, buffer);
    return errno;
}

int cd(char* words[] , int word_count){
    
    char* newdir = (char *) malloc (INP_SIZE);
    if(word_count == 1){
        chdir(HOME);
        return errno;
    }
    else if(word_count > 2)
    {
        printf("\n too many arguments for cd ");
        return 1;
    }
    else{
        if(words[1][0] == '~'){
            strcpy(newdir, HOME);
            printf("\n%s", HOME);
            strcat(newdir,words[1] + 1);
        }
        else if(words[1][0] == '-'){
            strcpy(newdir, LWD);
            printf("\n%s", newdir);
        }
        else{
            strcat(newdir, words[1]);
        }
    printf("\n%s", newdir);
    strcpy(LWD, PWD);
    int x = chdir(newdir);
    if ( x < 0 ) perror("cd ");
    }
    return 0;
}

char* permissions(char* file_name){
    struct stat st;
    mode_t perm = st.st_mode;
    char* permission_str = (char*) malloc(10);
    permission_str[0] = (perm & S_IRUSR) ? 'r' : '-';
    permission_str[1] = (perm & S_IWUSR) ? 'w' : '-';
    permission_str[2] = (perm & S_IXUSR) ? 'x' : '-';
    permission_str[3] = (perm & S_IRGRP) ? 'r' : '-';
    permission_str[4] = (perm & S_IWGRP) ? 'w' : '-';
    permission_str[5] = (perm & S_IXGRP) ? 'x' : '-';
    permission_str[6] = (perm & S_IROTH) ? 'r' : '-';
    permission_str[7] = (perm & S_IWOTH) ? 'w' : '-';
    permission_str[8] = (perm & S_IXOTH) ? 'x' : '-';
    permission_str[9] = '\0';
    return permission_str;
}

char* file_name_filter(char* file, int home_chk){
    char* final_name = (char *) malloc(INP_SIZE);
    strcpy(final_name, HOME);
    // printf("\n file : %s", file);
    if(file[0] == '~'){
        // printf("\n in home check");
        strcat(final_name , file + 1); 
    }
    else{
        strcpy(final_name, file);
    }
    // printf("\n file name : %s ", final_name);
    return final_name;
}

int print_l_ls(char* file_name , char* dir_name){
    struct stat f_info;
    char* final_dir_name = (char*) malloc (INP_SIZE);
    strcpy(final_dir_name, file_name);
    strcat(final_dir_name, "/");
    strcat(final_dir_name , dir_name);
    if( stat(final_dir_name, &f_info) != -1){
        struct group *group_d = getgrgid(f_info.st_gid);
        struct passwd *passwd_d = getpwuid(f_info.st_uid);
        char *time_c = (char *)malloc(100); 
        char* time_format = "%b %d %R";
        if(labs(time(NULL) - f_info.st_mtim.tv_sec) > (6*2629743)){
            time_format = "%b %d %Y";
        }
        strftime(time_c, 50, "%b %d %R", localtime( &f_info.st_mtime));
        printf("\n%s %ld %s %s %ld %s %s", permissions(final_dir_name), f_info.st_nlink , passwd_d->pw_name, group_d->gr_name , f_info.st_size , time_c , dir_name);
    }
    else{
        printf("\n name :  %s", final_dir_name);
    }
    return 0;
}

int printls(int a , int l, int relative,  char* f_name){
    
    char* file_name = file_name_filter(f_name , relative);
    DIR *open_dir = opendir(file_name);

    if (open_dir == NULL){
        perror("\nls");
        return 1;
    }
    
    printf("\n%s", file_name);
    struct dirent *sub_files = readdir(open_dir);
    while(sub_files != NULL){
        if(a == 1 && l == 0){
            printf("\n%s", sub_files->d_name);
        }
        else if(a == 0 && l == 0){
            if(sub_files->d_name[0] != '.'){
                printf("\n%s", sub_files->d_name);
            }
        }
        else if(a == 0 && l == 1 ){
            // printf("\nchk : %c", sub_files->d_name[0]);
            if(sub_files->d_name[0] != '.'){
                print_l_ls(file_name, sub_files->d_name);
            }
        }
        else if( a == 1 && l == 1){
            print_l_ls(file_name, sub_files->d_name);
        }
        sub_files = readdir(open_dir);

    } 

    return 0;
}

int ls(char* words[] , int word_count){

    int a_flag = 0, l_flag = 0, relative = 1;
    char* file_names[INP_SIZE];
    int indexes[INP_SIZE];
    for (int i = 0 ; i < INP_SIZE ; i++){
        file_names[i] = (char *) malloc (INP_SIZE);
    }
    int file_count = 0;
    for (int i = 1; i < word_count ; i ++){
        if (words[i][0] == '-'){
            int j = 1;
            while(words[i][j]){
                if( words[i][j] == 'a') a_flag = 1;
                if( words[i][j] == 'l') l_flag = 1;
                if( words[i][j] != 'l' && words[i][j] != 'a'){
                    printf("\nls : invalid flag");
                    return 1;
                }
                j++;
                
            }
            if ( j == 1 ){
                printf("\nls : invalid flag");
                return 1;
            }
        }
        else{
            strcpy(file_names[file_count] ,words[i]);
            indexes[file_count++] = i;
        }
    }
    if (file_count == 0){
        strcpy(file_names[0],PWD);
        relative = 0;
        file_count++;
    }
    for (int i = 0; i < file_count; i++ ){
        // printf("\nchk : %s ,a :  %d ,l :%d ,rel :  %d,fc:  %d ", file_names[i], a_flag, l_flag, relative , file_count);
        printls( a_flag, l_flag , relative ,  file_names[i] );
    }
    return 0;
}

int pwd(char* words[] , int word_count){
    if(word_count == 1){
        printf("\n%s", PWD);
    }
    else{
        printf("pwd : too many arguments");
    }
    return 0;
}

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

int repeat(char* words[] , int word_count){
    
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

/// finish these in 1 hour
int addchild(int pid, char* command){
    if (total_children_bg > 99){
        printf("\nadd process : too many children process, no space in pool");
        return 1;
    }

    cp[total_children_bg].child_id = pid;
    strcpy(cp[total_children_bg].childname, command);
    total_children_bg++;

    return 0;
}

int removechild(int pid){
    int i;
    int found = 0;
    printf("\nremovechilde children : %d", total_children_bg );
    for(i = 0; i< total_children_bg ; i++){
        printf("\nstruct : %d", cp[i].child_id);
        if(pid == cp[i].child_id){
            found = 1;
            break;
        }
    }
    if(found == 0){
        printf("\nremove process : could not locate child process");
        return 1;
    }
    struct child_process temp ;
    temp.child_id = cp[i].child_id;
    strcpy(temp.childname ,cp[i].childname);
    total_children_bg --;
    for( ; i < total_children_bg  ;i++){
        cp[i] = cp[i + 1];
    }
    return 0;
}

void signalhandler(int signal){
    
    pid_t pid;
    int pstat;
    printf("Hewewe");
    while ((pid = waitpid( -1, &pstat, WNOHANG)) > 0){
        pstat = WIFEXITED(pstat);
        printf("\nlollbahjbd hid");
        for (int i = 0; i < total_children_bg ; i++){
            if( cp[i].child_id == pid){
                printf("\n%d exited  %s", pid , (pstat == 0 )? "Normally" : "Abnormally");
                removechild(pid);
                
            }
        }
    }
    getPWD();
    fflush(stdout);

}

int fg(char* words[] , int word_count){
    
    int pid = fork();
    if( pid < 0){
        perror("\nforking ");
        return errno;
    }
    printf("\n fg  : %s", words[0]);
    if(pid == 0){
        int status = execvp(words[0], words);
        if(status < 0){
            perror("\nexecvp ");
            return 1;
        }

    }else{
        wait(NULL);
    }
    return 0;
}

int bg(char* words[] , int word_count){
    
    int v_words = word_count;
    printf("\n bg  : %s", words[0]);
    
    words[v_words - 1][strlen(words[v_words - 1]) - 1] = '\0';


    if (strcmp(words[v_words - 1],"\0") == 0){
        printf("\n bg : here");
        words[v_words - 1] = NULL;
        v_words --;
    }
    printf("\n bg : heretoo");
    for (int k =0 ; k < v_words; k++)
    {
        if(words[k] != NULL)
        printf("\nbg words: %s", words[k]);
    }
    printf("\n bg %s : here3", words[v_words - 1]);
    int pid = fork();

    if( pid < 0){
        perror("\nforking ");
        return errno;
    }
    if(pid == 0){
        printf("\n bg : here4");
        int status = execvp(words[0], words);
        if(status < 0){
            perror("\nexecvp ");
            return 1;
        }

    }else{
        printf("\n bg : here5");
        children_pid[total_children_bg] = pid;
        int errorchk = addchild(pid, words[0]);
        if(errorchk != 0){
            printf("\nerror in child process adding");
            return 1;
        }
        struct sigaction sigh;
        sigh.sa_handler = &signalhandler;
        sigemptyset(&sigh.sa_mask);
        sigh.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sigh, NULL) == -1){
            perror("couldnt signal the process");
        }
        sleep(1);
    }
    return 0;
}

int pinfo(char* words[] , int word_count){
    
    pid_t pid;
    
    if (word_count > 2){
        printf("\nInvalid arguments");
    }
    else if (word_count == 2){
        pid = atoi(words[1]);
    }
    else{
        pid = getpid();
    }

    char* proc_path = (char *) malloc (100);
    char* exec_path = (char *) malloc (100);
    sprintf(proc_path,"/proc/%d/stat", pid);
    sprintf(exec_path, "/proc/%d/exe", pid);
    FILE* process_info;
    process_info = fopen(proc_path, "r");
    char* full_proc_info = (char *) malloc (INP_SIZE);
    if( process_info == NULL) {
        printf("\npinfo : no such process ");
        return 1;
    }
    else
    {
        fread(full_proc_info, INP_SIZE, 1, process_info);


        char* file_stats[100] = {NULL}; 
        int i=0;
        file_stats[i] = strtok(full_proc_info, " ");
        while(file_stats[i]){
            file_stats[++i] = strtok(NULL, " ");
        }
        char exe_path[INP_SIZE ];
        memset(exe_path, 0, INP_SIZE - 1);
        int readStat = readlink(exec_path, exe_path, PATH_MAX);

        printf("\npid -- %d",pid);
        printf("\nProcess Status -- %s%s", file_stats[2], pid == getpid() ? "+":"");
        printf("\nmemory -- %s",file_stats[22]);
        printf("\nExecutable Path -- %s", readStat == -1 ? "Doesn't exist" : exe_path);
    }
    fclose(process_info);
    free(proc_path);
    free(exec_path);
    return 0;
}

int runcommand(char* words[] , int word_count){
    int rval = 0;
    if(strcmp(words[0], "cd") == 0){
        rval = cd(words, word_count);
    }
    else if(strcmp(words[0], "exit") == 0){
        addHistory(words, word_count);
        closeHistory();
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
    else if(strcmp(words[word_count - 1] + strlen(words[word_count - 1]) - 1, "&" ) == 0){
        rval = bg(words, word_count);
    }
    else{
        rval = fg(words, word_count);
    }
    

    return rval;
}

// function to tokenize and manage the input received
int manageINPUT(char* input){
    
    // splitting the input based on ;
    char* tokenised_input[INP_SIZE];
    for (int i = 0; i < INP_SIZE; i++){
        tokenised_input[i] = NULL;
    }

    char* command_index = strtok(input, ";");
    int i;
    for(i = 0; command_index ; i++){
        tokenised_input[i] = command_index;
        command_index = strtok(NULL, ";");
        // printf("\n%d : %s", i , tokenised_input[i]);
    }
    if(tokenised_input[0] == NULL && i == 0 ){
        printf("tokenised input last entry is o");
        return 0;
    }

    // splitting each string obtained into words
    int inp_commands = i;
    for(i = 0; i < inp_commands; i++){
        char* linewise_tok_input = tokenised_input[i];
        // printf("\n%d : %s", i , linewise_tok_input);
        char* words_in_line[INP_SIZE];
        
        for(int j = 0; j < INP_SIZE; j++){
            // words_in_line[j] = (char *) malloc (INP_SIZE);
            // strcpy(words_in_line[j],"");
            words_in_line[j] = NULL;
        }
        char* word_pointer = strtok(linewise_tok_input, " \t\n");

        int words;
        for ( words = 0; word_pointer ; words++ ){
            words_in_line[words] = word_pointer;
            word_pointer = strtok(NULL, " \t\n");
            // printf("\n%d , %d : %s",words, i , words_in_line[words]);
        }
        if(words == 0){
            printf("input : no entry");
            continue;
        }   

        int x = runcommand(words_in_line, words);
        if(x == 0);
            addHistory(words_in_line, words);

    }

    return 0;

}

int main(){


    initialise();
    char* input = (char*) malloc (INP_SIZE);
    
    while(1){
        
        getPWD();
        
        getINPUT(input);
        printf("\n%s", input);
        if(manageINPUT(input) != 0){
            printf("command : error");
        }
    }
    free(input);
    return 0;
}