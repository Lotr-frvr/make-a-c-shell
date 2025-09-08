#include "header_library.h"
#include "utils.h"

void pipe_handler(char *cmd)
{
    char *commands[100];
    int n = 0, p[2], fd_in = 0;

    while (*cmd == ' ') {
        cmd++; 
    } // Remove leading spaces

    if (*cmd == '|' || cmd[strlen(cmd) - 1] == '|') {
        printf("Error: Invalid use of pipe\n");
        return;
    }

    char *command = strtok(cmd, "|");
    while (command) {
        commands[n++] = command;
        command = strtok(NULL, "|");
    }
    commands[n] = NULL;

    for (int i = 0; i < n; i++) {
        pipe(p);
        if (fork() == 0) {
            dup2(fd_in, 0);
            if (commands[i + 1]) dup2(p[1], 1);
            close(p[0]);
            trim_command(commands[i]);
            input_utils(commands[i]);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            close(p[1]);
            fd_in = p[0];
        }
    }
}