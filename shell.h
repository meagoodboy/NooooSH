#ifndef SHELL
#define SHELL 
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

    int total_children_bg;
    int history_count;
    // int curr_fgp;
    char* history_list[20];
    // extern int children_pid[1000];
    typedef struct child_process{
        char childname[1000];
        int child_id;
        int status;
    } cp_struct;
    cp_struct cp[100];
    cp_struct fgpi;
    int errno;
    char USER[1000];
    char HOST[1000];
    char HOME[1000];
    char PWD[1000];
    char LWD[1000];


    int runcommand(char* words[] , int word_count);
    int removechild(int pid);
    int cd(char* words[] , int word_count);
    int addHistory(char* words[] , int word_count);
    int ls(char* words[] , int word_count);
    int pwd(char* words[] , int word_count);
    int pinfo(char* words[] , int word_count);
    int getHistory(char* words[] , int word_count);
    int bg(char* words[] , int word_count);
    int addchild(int pid, char* command);
#endif