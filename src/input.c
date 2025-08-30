#include "header_library.h"
#include "utils.h"


void run_external_command(char* command, char * args[] ) {
    // this should be handles as foreground and background processes
    printf("Running external command: %s\n", command);
    printf("With arguments:\n");
    for (int i = 0; args[i] != NULL; i++) {
        printf("  %s\n", args[i]);
    }
}


void execute_command(char* command, char * args[] ) {
    if(strcmp(command, "exit") == 0) {
        exit(0);
    }
    else if(strcmp(command,"hop")==0){
        hop(args);
    }
    else if(strcmp(command,"reveal")==0){
        reveal(args);
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

void input_utils(char* input_command){
    char *saveptr;
    char *token = strtok_r(input_command, ";&", &saveptr);
    while (token != NULL) {
        // Remove leading/trailing whitespace
        while (*token == ' ' || *token == '\t') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && (*end == ' ' || *end == '\t' || *end == '\n')) {
            *end = '\0';
            end--;
        }
        if (strlen(token) > 0) {
            // Tokenize command and arguments
            char *args[ARGS_COMMAND];
            int argc = 0;
            char *arg_saveptr;
            char *arg_token = strtok_r(token, " \t", &arg_saveptr);
            while (arg_token != NULL && argc < ARGS_COMMAND - 1) {
                args[argc++] = arg_token;
                arg_token = strtok_r(NULL, " \t", &arg_saveptr);
            }
            args[argc] = NULL;
            if (argc > 0) {
                if (is_custom_command(args[0])) {
                    execute_command(args[0], args);
                } else {
                    run_external_command(args[0], args);
                }
            }
        }
        token = strtok_r(NULL, ";&", &saveptr);
    }
}

void input(){
    char *input_command = NULL;
    size_t len = 0;
    if(getline(&input_command, &len, stdin) == -1){ 
        // input wasnt read properly
        fprintf(stderr, "Error reading input\n");
    }
    input_utils(input_command);
    free(input_command);
}

