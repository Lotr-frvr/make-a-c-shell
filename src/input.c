#include "header_library.h"
#include "utils.h"

char* strip_quotes(char* str) {
    if (!str) return str;
    int len = strlen(str);
    if (len >= 2) {
        if ((str[0] == '"' && str[len - 1] == '"') ||
            (str[0] == '\'' && str[len - 1] == '\'')) {
            str[len - 1] = '\0';   // remove last quote
            return str + 1;        // skip first quote
        }
    }
    return str;
}

void run_external_command(char* command, char * args[], int bg_bit) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }
    else if (pid == 0) {
        if (bg_bit) {
            setpgid(0, 0); // Set a new process group for background process
        }
        execvp(command, args);
        // If execvp returns, an error occurred
        fprintf(stderr, "Error: failed to execute '%s': %s\n", command, strerror(errno));
        exit(1);
    }
    else {
        if (bg_bit) {
            bg_function(command, args, pid); // args[0]=command
        }
        else {
            fg_function(command, args, pid);
        }
    }
}


void execute_command(char* command, char * args[] ) {
    if(strcmp(command, "exit") == 0) {
        cleanup_bg_processes();
        exit(0);
    }
    else if(strcmp(command,"hop")==0){
        hop(args);
    }
    else if(strcmp(command,"reveal")==0){
        reveal(args);
    }
    else if(strcmp(command,"log")==0){
        log_command(args);
    }
    else if(strcmp(command,"seek")==0){
        seek(args);
    }
    else if(strcmp(command,"proclore")==0){
        proclore(args);
    }
    else if (strcmp(command, "activities") == 0) {
        activities();
    }
    else if(strcmp(command, "ping") == 0) {
        ping(args);
    }
    else if(strcmp(command, "iMan") == 0) {
        iMan(args);
    }
    else if(strcmp(command, "fg") == 0) {
        bgtofg(args);
    }
    else if(strcmp(command, "bg") == 0) {
        fgtobg(args);
    }
    else{
        printf(ERROR_COLOR"Command not found: %s\n"RESET, command);
    }

}
int is_custom_command(char* command) {
    // Check if the command is a custom command
    if (strcmp(command, "hop") == 0 || strcmp(command, "reveal") == 0 ||
        strcmp(command, "log") == 0 || strcmp(command, "exit") == 0 ||
        strcmp(command, "seek") == 0 || strcmp(command, "proclore") == 0 ||
        strcmp(command, "activities") == 0 || strcmp(command, "fg") == 0 ||
        strcmp(command, "bg") == 0 || strcmp(command, "neonate") == 0 ||
        strcmp(command, "iMan") == 0 || strcmp(command, "ping") == 0) {
        return 1;
    }
    
    return 0;
}

void input_utils(char* input_command) {
    int len = strlen(input_command);
    int i = 0;

    while (i < len) {
        // Skip whitespace
        while (i < len && (input_command[i] == ' ' || input_command[i] == '\t' || input_command[i] == '\n'))
            i++;
        if (i >= len) break;

        // Find command boundaries
        int start = i;
        int bg_bit = 0;
        while (i < len && input_command[i] != ';' && input_command[i] != '&')
            i++;
        int end = i;

        // Background flag
        if (i < len && input_command[i] == '&')
            bg_bit = 1;

        // Extract command
        char cmd[1024];
        strncpy(cmd, input_command + start, end - start);
        cmd[end - start] = '\0';

        // Trim
        char *token = cmd;
        while (*token == ' ' || *token == '\t') token++;
        char *cmd_end = token + strlen(token) - 1;
        while (cmd_end > token && (*cmd_end == ' ' || *cmd_end == '\t' || *cmd_end == '\n')) {
            *cmd_end = '\0';
            cmd_end--;
        }

        if (strlen(token) > 0) {
            // --- Handle pipes first ---
            if (strchr(token, '|')) {
                pipe_handler(token);
            }
            // --- Handle redirection ---
            else if (strchr(token, '<') || strstr(token, ">>") || strchr(token, '>')) {
                redirection_handler(token);
            }
            // --- Normal execution ---
            else {
                char *args[ARGS_COMMAND];
                int argc = 0;
                char *arg_saveptr;
                char *arg_token = strtok_r(token, " \t", &arg_saveptr);
                while (arg_token != NULL && argc < ARGS_COMMAND - 1) {
                    args[argc++] = strip_quotes(arg_token);   // <-- strip here
                    arg_token = strtok_r(NULL, " \t", &arg_saveptr);
                }
                args[argc] = NULL;

                if (argc > 0) {
                    if (is_custom_command(args[0])) {
                        execute_command(args[0], args);
                    } else {
                        run_external_command(args[0], args, bg_bit);
                    }
                }
            }
        }

        // Move past delimiter
        i++;
    }
}

void input(){
    char *input_command = NULL;
    size_t len = 0;
    if(getline(&input_command, &len, stdin) == -1){ 
        // input wasnt read properly
        if(feof(stdin)) {
            ctrld(); // Handle Ctrl-D (EOF)
        } else {
            fprintf(stderr, "Error reading input\n");
        }
        
    }
    log_write_command_to_file(input_command);  // Log the command

    input_utils(input_command);
    free(input_command);
}

