#include"header_library.h"
#include "utils.h"

char* trim_command(char* cmd) {
    int i = 0, j = 0;
    int len = strlen(cmd);

    if (cmd[0] == '\n') {
        return "";
    }

    while (i < len && isspace(cmd[i])) {
        i++;
    }

    while (i < len) {
        if (!isspace(cmd[i])) {
            cmd[j++] = cmd[i];
        } else if (isspace(cmd[i]) && !isspace(cmd[i - 1])) {
            cmd[j++] = cmd[i];
        }
        i++;
    }

    if (j > 0 && isspace(cmd[j - 1])) {
        j--;
    }

    cmd[j] = '\0';
    return cmd;
}

void redirection_handler(char *command) {
    trim_command(command);
    // Execute the command
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    } else if (pid == 0) {
        char *args[ARGS_COMMAND];  // Tokenized command arguments
    int i = 0;
    int input_fd = -1, output_fd = -1;
    int append = 0; // To track if >> is used

    // Tokenize the command
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Handling input and output redirection
    for (int j = 0; args[j] != NULL; j++) {
        if (strcmp(args[j], "<") == 0) {
            // Input redirection
            input_fd = open(args[j + 1], O_RDONLY);
            if (input_fd < 0) {
                perror("No such input file found");
                return;
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
            args[j] = NULL;  // Remove redirection from args
        }  else if (strcmp(args[j], ">>") == 0) {
            // Output redirection (append)
            output_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd < 0) {
                perror("Error opening output file");
                return;
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
            args[j] = NULL;  // Remove redirection from args
            // then adjust the index to the next argument
            //args[j]=
            append++;
        }
        else if (strcmp(args[j], ">") == 0) {
            // Output redirection (overwrite)
            output_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd < 0) {
                perror("Error opening output file");
                return;
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
            args[j] = NULL;  // Remove redirection from args
        }
    }


        if (execvp(args[0], args) == -1) {
            perror("Exec failed");
            exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}