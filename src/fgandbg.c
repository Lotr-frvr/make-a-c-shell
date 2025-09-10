#include "header_library.h"
#include "utils.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern int bgi;
extern struct backproc *bgs;

extern int currfgid;
extern int currfgtime;
extern char currfgcom[CURRFG_SIZE];

void bgtofg(char* args[]) {
    if (args[1] == NULL) {
        printf("fg: missing process id\n");
        return;
    }

    int pid = atoi(args[1]);
    int found = 0;
    int index;

    // Search for process in background list
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

    // Give terminal control to the process
    pid_t pgid = getpgid(pid);
    pid_t shell_pgid = getpgrp();

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, pgid) == -1) {
        perror("tcsetpgrp");
    }

    // Continue the process
    kill(pid, SIGCONT);

    // Wait for it to finish or stop
    int status;
    waitpid(pid, &status, WUNTRACED);

    // Remove from background list if finished
    if (WIFEXITED(status) || WIFSIGNALED(status)) {
        for (int j = index; j < bgi - 1; j++)
            bgs[j] = bgs[j + 1];
        bgi--;
    }


    // Restore terminal control to shell
    tcsetpgrp(STDIN_FILENO, shell_pgid);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}

void fgtobg(char* args[]) {
    if (args[1] == NULL) {
        printf("bg: missing process id\n");
        return;
    }

    int pid = atoi(args[1]);
    int found = 0;
    int index;

    // Search for process in background list
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

    // Resume the process in the background
    if (kill(pid, SIGCONT) == -1) {
        perror("kill");
        return;
    }

    printf("%d %s - Running\n", pid, bgs[index].name);
}
