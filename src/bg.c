#include "header_library.h"
#include "utils.h"
extern int bgi;
extern struct backproc *bgs;

void bg_exit_handler(int signum){
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Find process name
        char proc_name[CURRFG_SIZE] = "Process";
        for (int i = 0; i < bgi; i++) {
            if (bgs[i].pid == pid) {
                strncpy(proc_name, bgs[i].name, CURRFG_SIZE - 1);
                proc_name[CURRFG_SIZE - 1] = '\0';
                // Remove from list
                for (int j = i; j < bgi - 1; j++)
                    bgs[j] = bgs[j + 1];
                bgi--;
                break;
            }
        }
        if (WIFEXITED(status)) {
            printf("\n%s exited normally (%d)\n", proc_name, pid);
        } else {
            printf("\n%s exited abnormally (%d)\n", proc_name, pid);
        }
        fflush(stdout);
    }
}

void bg_function(char* command, char* args[], pid_t pid) {
    printf("%d\n",pid);
    if(bgs==0){
        bgs = malloc(sizeof(struct backproc) * BG_MAX); // or a suitable max
    }
    bgs[bgi].pid = pid;
    strncpy(bgs[bgi].name, command, CURRFG_SIZE - 1);
    bgs[bgi].name[CURRFG_SIZE - 1] = '\0';
    bgi++;
}