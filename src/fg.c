#include "header_library.h"
#include "utils.h"
extern int currfgid;
extern int currfgtime;
extern char currfgcom[CURRFG_SIZE];

void fg_function(char* command, char* args[], pid_t pid) {
    time_t start_time = time(NULL);
    int status;

    currfgid = pid;
    strncpy(currfgcom, command, CURRFG_SIZE - 1);
    currfgcom[CURRFG_SIZE - 1] = '\0';

    waitpid(pid, &status, 0);

    time_t end_time = time(NULL);
    int duration = (int)(end_time - start_time);

    if (duration > 2) {
        currfgtime = duration;
    } else {
        currfgtime = 0; // Don't display in prompt if <= 2 seconds
    }
}