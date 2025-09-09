#include "header_library.h"
#include "utils.h"
extern int currfgid;
extern int currfgtime;
extern char currfgcom[CURRFG_SIZE];
extern int bgi;
extern struct backproc *bgs;
/* 
 * ping <pid> <signal_number>
 * Sends signal_number (mod 32) to process with given pid.
 *
 * Common signal numbers for reference:
 *   1  SIGHUP    (Hangup)
 *   2  SIGINT    (Interrupt from keyboard)
 *   9  SIGKILL   (Kill, cannot be caught or ignored)
 *  15  SIGTERM   (Termination)
 *  17  SIGCHLD   (Child stopped or terminated)
 *  18  SIGCONT   (Continue if stopped)
 *  19  SIGSTOP   (Stop process, cannot be caught or ignored)
 */
void ping(char* args[]) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: ping <pid> <signal_number>\n");
        return;
    }

    pid_t pid = atoi(args[1]);
    int sig = atoi(args[2]) % 32;

    if (kill(pid, 0) < 0) {
        perror("No such process found");
        return;
    }

    if (kill(pid, sig) == 0) {
        printf("Sent signal %d to process with pid %d\n", sig, pid);
    } else {
        perror("Failed to send signal");
    }
}

/* 
 * Ctrl-C (SIGINT) 
 * Interrupts currently running foreground process.
 */
void ctrlc(int signum) {
    if (currfgid > 0) { // currfgid should track foreground process
        kill(currfgid, SIGINT);
        printf("\nInterrupted process %d with SIGINT\n", currfgid);
    } else {
        printf("\n");
    }
}

/* 
 * Ctrl-D (EOF) 
 * Exits shell after killing all background processes.
 */
void ctrld() {
    for (int i = 0; i < bgi; i++) {
        if (bgs[i].pid > 0) {
            kill(bgs[i].pid, SIGKILL);
        }
    }
    printf("Exiting shell...\n");
    exit(0);
}

/* 
 * Ctrl-Z (SIGTSTP) 
 * Moves current foreground process to background in stopped state.
 */
void ctrlz(int signum) {
    if(bgs==0){
        bgs = malloc(sizeof(struct backproc) * BG_MAX); 
    }
    if (currfgid > 0) {
        if (kill(currfgid, SIGTSTP) == 0) {
            
            printf("Stopped process %d with SIGTSTP\n", currfgid);
            strcpy(bgs[bgi].name, currfgcom);
            bgs[bgi].pid = currfgid;
            bgi++;

            currfgid = 0; // reset foreground

            write(STDOUT_FILENO, "\nProcess moved to background (Stopped)\n", 39);
            return ;
        }
    } else {
        write(STDOUT_FILENO, "\n", 1);
    }
}
