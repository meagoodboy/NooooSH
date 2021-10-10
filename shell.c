#include "shell.h"
#include "commands/commands.h"


int total_children_bg = 0;
int history_count = 0;
char* history_list[20] = {NULL};
int children_pid[1000];
cp_struct cp[100];
int errno;
char USER[1000];
char HOST[1000];
char HOME[1000];
char PWD[1000];
char LWD[1000];
int curr_fgp = -1;



// initialise history
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
        // printf("\nHistory iniitalised sucessfullly here");
        history_count = 0;
        fread(temp_history, 1, 20 * INP_SIZE, history_loc);
        // printf("\nhere : %s", temp_history);
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

// add command to history and update file
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
    return 0;
}

// function to print history
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
    printf("   wElCoMe To ThE NooooSH");
    printf("\n ************************************************************ \n\n");
    reset_color();
    initHistory();
    getcwd(PWD, sizeof(PWD));
    strcpy(USER, getenv("USER"));
    strcpy(HOME, PWD);
    
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

// kill all child process
int killall(){
    for(int i = 0 ; i < total_children_bg ; i++){
        kill(cp[i].child_id, SIGKILL);
    }
    return 0;
}

// function to format input to the terminal
int getINPUT(char* input_str){


    char* buffer = NULL;     
    size_t bufr_size_m = 0;                           
    int chk = getline(&buffer, &bufr_size_m, stdin);
    if(chk == -1){
        killall();
        exit(0);
        // return 9;
    }
    // printf("%s", buffer);
    // perror("lol");
    strcpy(input_str, buffer);
    return errno;
}

// cd command
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
            printf("~\n");
            strcat(newdir,words[1] + 1);
        }
        else if(words[1][0] == '-'){
            strcpy(newdir, LWD);
            // printf("%s    %s\n", formatpath(), PWD);
        }
        else{
            strcat(newdir, words[1]);
        }
    // printf("\n%s", newdir);
        strcpy(LWD, PWD);
        int x = chdir(newdir);
        getcwd(PWD, sizeof(PWD));
        if(words[1][0] == '-'){
            printf("%s\n", formatpath());
        }
        if ( x < 0 ) perror("cd ");
    }
    return 0;
}

// to return the permissions of a given file
char* permissions(char* file_name){
    struct stat st;
    stat(file_name,&st);
    mode_t perm = st.st_mode;
    // printf("\n permisssions :  %s", file_name);
    char* permission_str = (char*) malloc(11);
    permission_str[0] = (S_ISDIR(perm)) ? 'd' : '-';
    permission_str[1] = (perm & S_IRUSR) ? 'r' : '-';
    permission_str[2] = (perm & S_IWUSR) ? 'w' : '-';
    permission_str[3] = (perm & S_IXUSR) ? 'x' : '-';
    permission_str[4] = (perm & S_IRGRP) ? 'r' : '-';
    permission_str[5] = (perm & S_IWGRP) ? 'w' : '-';
    permission_str[6] = (perm & S_IXGRP) ? 'x' : '-';
    permission_str[7] = (perm & S_IROTH) ? 'r' : '-';
    permission_str[8] = (perm & S_IWOTH) ? 'w' : '-';
    permission_str[9] = (perm & S_IXOTH) ? 'x' : '-';
    permission_str[10] = '\0';
    return permission_str;
}

// to get the correct path in file name
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

// print the ls -l
int print_l_ls(char* file_name , char* dir_name){
    struct stat f_info;
    char* final_dir_name = (char*) malloc (INP_SIZE);
    strcpy(final_dir_name, file_name);
    strcat(final_dir_name, "/");
    // int block_count = 0;
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
        // printf("\n%s   %s", final_dir_name, permissions(final_dir_name));
        // block_count += f_info.st_blocks;
        printf("\n%s %ld %s %s %ld %s %s", permissions(final_dir_name), f_info.st_nlink , passwd_d->pw_name, group_d->gr_name , f_info.st_size , time_c , dir_name);
        // printf(" %d", block_count);
    }
    else{
        printf("\n name :  %s", final_dir_name);
    }
    return 0;
}

int get_l_ls_total(char* file_name , char* dir_name){
    struct stat f_info;
    char* final_dir_name = (char*) malloc (INP_SIZE);
    strcpy(final_dir_name, file_name);
    strcat(final_dir_name, "/");
    int block_count = 0;
    strcat(final_dir_name , dir_name);
    if( stat(final_dir_name, &f_info) != -1){
        block_count = f_info.st_blocks;
    }
    else{
        printf("\n name :  %s", final_dir_name);
    }
    return block_count;
}


// funtion to print normal ls
int printls(int a , int l, int relative,  char* f_name){
    
    char* file_name = file_name_filter(f_name , relative);
    DIR *open_dir = opendir(file_name);
    DIR *count_dir = opendir(file_name);
    if (open_dir == NULL){
        perror("\nls");
        printf("\n%s", file_name);
        return 1;
    }
    
    // printf("\n%s", file_name);
    struct dirent *sub_files = readdir(open_dir);
    struct dirent *count_file = readdir(count_dir);
    if( l != 0 ){
        int total_blk = 0;
        while(count_file != NULL){

            if(a == 0 && l == 1 ){
                if(count_file->d_name[0] != '.'){
                    total_blk += get_l_ls_total(file_name, count_file->d_name);

                }
            }
            else if( a == 1 && l == 1){
                total_blk += get_l_ls_total(file_name, count_file->d_name);
            }


            // printf("\n %d", total_blk);
            count_file = readdir(count_dir);
        } 

        printf("total %d", total_blk/2);
    }


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

// function to get all the files
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
    printf("\n");
    return 0;
}

// print pwd
int pwd(char* words[] , int word_count){
    if(word_count == 1){
        printf("%s\n", PWD);
    }
    else{
        printf("pwd : too many arguments");
    }
    return 0;
}


/// to add a child process
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
// tto remove a child process
int removechild(int pid){
    int i;
    int found = 0;
    // printf("\nremovechilde children : %d", total_children_bg );
    for(i = 0; i< total_children_bg ; i++){
        // printf("\nstruct : %d", cp[i].child_id);
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

// to handle bg signals
void signalhandler(int signal){
    
    pid_t pid;
    int pstat;
    // printf("Hewewe");
    while ((pid = waitpid( -1, &pstat, WNOHANG)) > 0){
        pstat = WIFEXITED(pstat);
        // printf("\nlollbahjbd hid");
        for (int i = 0; i < total_children_bg ; i++){
            if( cp[i].child_id == pid){
                printf("\n%s with pid %d exited %s",cp[i].childname, pid , (pstat == 1 )? "Normally" : "Abnormally");
                removechild(pid);
                
            }
        }
    }
    getPWD();
    fflush(stdout);

}


// run background
int bg(char* words[] , int word_count){
    
    int v_words = word_count;
    // printf("\n bg  : %s", words[0]);
    char* temp_words[INP_SIZE];
    for (int i = 0; i < INP_SIZE ; i ++){
        temp_words[i] = (char*) malloc(INP_SIZE);
    }
    
    words[v_words - 1][strlen(words[v_words - 1]) - 1] = '\0';

    if (strcmp(words[v_words - 1],"\0") == 0){
        for(int i = 0; i < v_words - 1; i++){
            strcpy(temp_words[i], words[i]);
        }
        temp_words[v_words - 1] = NULL;
        v_words --;
    }
    else{
        for(int i = 0; i < v_words ; i++){
            strcpy(temp_words[i], words[i]);
        }
        temp_words[v_words] = NULL;
    }
    int pid = fork();

    if( pid < 0){
        perror("\nforking ");
        return errno;
    }
    if(pid == 0){
        // printf("\n bg : here4");
        setpgid(0 ,0);
        int status = execvp(temp_words[0], temp_words);
        if(status < 0){
            perror("\nexecvp ");
            return 1;
        }

    }else{
        printf("\n%d " ,pid);
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

// print process info
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

// run all commands
int runcommand(char* words[] , int word_count){
    int rval = 0;

    int rd_chk = check_redirection(words, word_count);

    if(rd_chk == 1){
        manage_redirection(words, word_count);
    }
    else{
    rval = runcommand_f(words, word_count);
    }
    
    // fprintf(stderr, "exited redir\n");
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
        // printf("tokenised input last entry is 0");
        return 0;
    }

    // splitting each string obtained into words
    int inp_commands = i;
    for(i = 0; i < inp_commands; i++){
        char* linewise_tok_input = tokenised_input[i];
        // printf("\n%d : %s", i , linewise_tok_input);


        // checking for pipes
        // int pipecomm = 0;
        int pipec = 0;
        // char* pipepointer = strtok(linewise_tok_input, "|");
        int len = strlen(linewise_tok_input);
        for(int m = 0; m < len; m++){
            if(linewise_tok_input[m] == '|'){
                pipec ++;
            }
        }

        if(pipec > 0){
            handlepipes(linewise_tok_input, pipec);
            continue;
        }
        else
        {
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
                // printf("input : no entry");
                continue;
            }   

            int x = runcommand(words_in_line, words);
            // fprintf(stderr,"here after running\n");
            if(x == 0)
                addHistory(words_in_line, words);
        }
    }

    return 0;

}

void inthandler(){
    ;
}

void stophandler(){
    // printf("\nstop handler");
    pid_t pid = fgpi.child_id;
    if (pid != -1){
    // kill(pid, SIGTSTP);
    // setpgid()
    addchild(pid , fgpi.childname);
    }
    // printf("\n in stop : %d", pid);
}

void addsignal(){
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = inthandler;
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);
    
    struct sigaction action2;
    memset(&action2, 0, sizeof(action2));
    action2.sa_handler = stophandler;
    action2.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &action2, NULL);
}

int main(){


    initialise();
    addsignal();
    char* input = (char*) malloc (INP_SIZE);
    int chk2 = 0;
    while(1){
        int stdinbkp = dup(STDIN_FILENO);
        int stdoutbkp = dup(STDOUT_FILENO);
        fgpi.child_id = -1;

        // to print prompt
        getPWD();
        
        chk2 = getINPUT(input);
        if (chk2 == 9){ 
            // printf("here");
            break;
        }
        // printf("\n%s", input);
        if(manageINPUT(input) != 0){
            printf("command : error");
        }
        dup2(stdinbkp, STDIN_FILENO);
        dup2(stdoutbkp, STDOUT_FILENO);
    }
    free(input);
    return 0;
}