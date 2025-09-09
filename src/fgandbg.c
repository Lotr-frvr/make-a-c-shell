#include "header_library.h"
#include "utils.h"
extern int bgi;
extern struct backproc *bgs;

extern int currfgid;
extern int currfgtime;
extern char currfgcom[CURRFG_SIZE];


void fgtobg(char* args[]) {
    if (args[1] == NULL) {
        printf("fg: missing process id\n");
        return;
    }

    int pid = atoi(args[1]);
    int found = 0;
    // Check if the process is currently in the foreground
    if (getpgid(pid) == getpgrp()) {
        for (int i = 0; i < bgi; i++) {
            if (bgs[i].pid == pid) {
                found = 1;

                // If the process is stopped, continue it in the background
                if (kill(pid, 0) == 0) {
                    kill(pid, SIGCONT); // Resume the process if it was stopped
                }
                // Add to background process list the fg process name and pid
                strcpy(bgs[i].name, currfgcom);
                bgs[i].pid = pid;
                bgi++;
                printf("Process with PID %d moved to background.\n", pid);

                // Update process state
                int status;
                waitpid(pid, &status, WNOHANG | WUNTRACED);

                if (WIFSTOPPED(status)) {
                    printf("Process %d stopped and moved to background.\n", pid);
                } else {
                    printf("Process %d running in background.\n", pid);
                }

                break;
            }
        }

        if (!found) {
            printf("No such process found\n");
        }
    } else {
        printf("Process %d is not in the foreground\n", pid);
    }
}

void bgtofg( char* args[]){
    if (args[1] == NULL) {
        printf("bg: missing process id\n");
        return;
    }

    int pid = atoi(args[1]);
    int found = 0;
    int index;
    
    // Search for the process in the background list
    for (index = 0; index < bgi; index++) {
        if (bgs[index].pid == pid) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("No such process found\n");
        return;
    }

    // Set signals to default handling
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    // Get the process group ID
    pid_t pgid = getpgid(pid);
    pid_t shell_pgid = getpgid(0);

    // Move the process to the foreground
    if (tcsetpgrp(STDIN_FILENO, pgid) == -1) {
        perror("tcsetpgrp");
        return;
    }

    // Continue the process
    if (kill(pid, SIGCONT) == -1) {
        perror("kill");
        return;
    }

    // Wait for the process to complete or be stopped again
    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1) {
        perror("waitpid");
        return;
    }

    // Remove the process from the background list if it has completed
    if (WIFEXITED(status) || WIFSIGNALED(status)) {
        for (int j = index; j < bgi - 1; j++) {
            bgs[j] = bgs[j + 1];
        }
        bgi--;
        printf("Process %d has completed and removed from background list.\n", pid);
    } else if (WIFSTOPPED(status)) {
        printf("Process %d is stopped.\n", pid);
    }

    // Restore control to the shell
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, shell_pgid) == -1) {
        perror("tcsetpgrp");
    }

    // Restore default signal handling
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}