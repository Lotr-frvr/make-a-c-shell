#ifndef UTILS_H__
#define UTILS_H__

#include"header_library.h"
/* prompt.c */
void prompt(char oghome_path[]);


/* input.c */
void input();
void input_utils(char* input_command);
void run_external_command(char* command, char *args[],int bg_bit);
void execute_command(char* command, char *args[]);
int is_custom_command(char* command);


/* log.c */
void log_command_execute(int index);
void log_write_command_to_file(char* cmd);
void log_command(char* args[]);
char* get_log_file_path();
void log_purge();


/* hop.c */
void hop(char* args[]);

 /* reveal.c */
 void reveal(char* args[]);
 void display_file_info(char *file_name, char *file_path, int show_hidden, int show_long);


// system commands
struct backproc{
    pid_t pid;
    char name[CURRFG_SIZE];
};

/*seek.c*/
void dfs(char rel_dir[], char abs_path[], char search_file[], int flag_id);
void seek(char* args[]);
int has_permission(const char *path, int is_dir);
void color_print(const char *path, int is_dir);

/*proclore.c*/
void proclore(char* args[]);
void print_Process_info(pid_t pid);


/*pipe.c*/
void pipe_handler(char *cmd);
/*redirect.c*/
void redirection_handler(char *command);
char* trim_command(char* cmd);

/*activities.c*/
void activities();

extern int bgi; // index of current background process
extern struct backproc *bgs; // list of background processes



/*fg.c*/
void fg_function(char* command, char* args[], pid_t pid);

/*bg.c*/
void bg_function(char* command, char* args[], pid_t pid);
void bg_exit_handler(int signum);
void cleanup_bg_processes();

/*neonate.c*/
void neonate(char* args[]);

/*ping.c*/
void ping(char* args[]);
void ctrlc();
void ctrld();
void ctrlz();


/*fgandbg*/
void fgtobg( char* args[]);
void bgtofg( char* args[]);



#endif