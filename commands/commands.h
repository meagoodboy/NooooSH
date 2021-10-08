#ifndef COMMANDS
#define COMMANDS

int echo(char* words[] , int word_count);
int fg(char* words[] , int word_count);
int repeat(char* words[] , int word_count);
int jobs(char* words[] , int word_count);
int sig(char* words[] , int word_count);
int fg_p(char* words[] , int word_count);
int bg_p(char* words[] , int word_count);
int handlepipes(char* input_line , int pipe_count);
int check_redirection(char* words[] , int word_count);
int manage_redirection(char* words[] , int word_count);
int runcommand_f(char* words[] , int word_count);

#endif