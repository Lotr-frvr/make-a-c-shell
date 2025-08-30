#ifndef UTILS_H__
#define UTILS_H__

#include"header_library.h"
/* prompt.c */
void prompt(char oghome_path[]);


/* input.c */
void input();
void input_utils(char* input_command);
void run_external_command(char* command, char *args[]);
void execute_command(char* command, char *args[]);
int is_custom_command(char* command);


// /* log.c */
// void log_command_execute(int index);
// void log_write_command_to_file(char* cmd);
// void log_command(char* args[]);
// char* get_log_file_path();
// void log_purge();

// /*signal.c*/

/* hop.c */
void hop(char* args[]);

 /* reveal.c */
 void reveal(char* args[]);
 void display_file_info(char *file_name, char *file_path, int show_hidden, int show_long);

// /*seek.c*/
// void dfs(char rel_dir[], char abs_path[], char search_file[], int flag_id);
// void seek(char* args[]);


// /*proclore.c*/
// void proclore(char* args[]);


// /*systemcall.c*/
// void systemcall(char* command,char *args[]);

// /*pipe.c*/
// void pipe_handler(char* cmd);

// /*redirect.c*/
// void redirection_handler(char* cmd);

// /*activities.c*/


// extern int bgi; // index of current background process
// struct backproc
// {
//     /* data */
//     int id;
//     char comm[4096];
// };
// extern struct backproc *bgs; // list of background processes


// void display_processes(char* args[]);
// int compareComm(const void *a, const void *b);

// /*iMan.c*/
// void iMan(char* args[]);


// /*fg.c*/
// void fg_function(char* command, char* args[]);
// //extern char* currfgcomm;
// //extern int currfgid;

// /*bg.c*/
// //extern int bg_count;
// void bg_function(char* command, char* args[]);

// void delete_bg(int id);
// void sigchld_handler(int signum);
// void handle_bg_termination();
// void setup_signal_handler();

// /*neonate.c*/

// /*ping.c*/
// void ping(char* args[]);
// void ctrlc();
// void ctrld();
// void ctrlz();


// /*fgtotbg*/
// void fgtobg( char* args[]);
// void bgtofg( char* args[]);



#endif