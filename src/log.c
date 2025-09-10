#include "header_library.h"
#include "utils.h"



// Check and create the log file if it doesn't exist

char* get_log_file_path() {
    extern char home_path[PATH_SIZE];
    char* home_dir =(char*) malloc(sizeof(char)*PATH_SIZE);
    strcpy(home_dir, home_path);
    if (home_dir == NULL) {
        printf("Error: HOME_DIR not set.\n");
        return NULL;
    }

    char *log_file_path = (char *) malloc(strlen(home_dir) + strlen(LOG_FILENAME) + 2);  // +2 for '/' and '\0'
    if (log_file_path == NULL) {
        printf("Error allocating memory for log file path.\n");
        return NULL;
    }

    strcpy(log_file_path, home_dir);
    strcat(log_file_path, "/");
    strcat(log_file_path, LOG_FILENAME);

    return log_file_path;
}

// Check and create the log file if it doesn't exist
void checkandcreatefile() {
    char *log_file_path = get_log_file_path();
    if (log_file_path == NULL) {
        return;
    }

    FILE *file = fopen(log_file_path, "r");
    if (file == NULL) {
        file = fopen(log_file_path, "w");
        if (file == NULL) {
            printf("Error creating log file\n");
            free(log_file_path);
            return;
        }
        fclose(file);
    } else {
        fclose(file);
    }

    free(log_file_path);
}

// Get the last line in the log file
char* get_last_line() {
    char *log_file_path = get_log_file_path();
    if (log_file_path == NULL) {
        return NULL;
    }

    FILE *file = fopen(log_file_path, "r");
    free(log_file_path);  // Free after using the path
    if (file == NULL) {
        printf("Error opening log file\n");
        return NULL;
    }

    char *last_line = (char *) malloc(LOG_LINE_MAX * sizeof(char));
    char *temp = (char *) malloc(LOG_LINE_MAX * sizeof(char));
    last_line[0] = '\0';

    while (fgets(temp, LOG_LINE_MAX, file) != NULL) {
        strcpy(last_line, temp);
    }

    free(temp);
    fclose(file);
    return last_line;
}

// Write a new command to the log file
void log_write_command_to_file(char *command) {
    if(command == NULL || command[0] == '\0' || strcmp(command, "\n") == 0) {
        return;
    }
    if(strlen(command) == 0) {
        return;
    }
    checkandcreatefile();  // Ensure log file exists

    if (strstr(command, "log") != NULL) {
        return;  // Skip logging if the command contains "log"
    }

    char *last_line = get_last_line();
    if (last_line == NULL) {
        return;
    }

    // Ensure that it's not a duplicate of the last line
    if (strcmp(last_line, command) != 0) {
        char *log_file_path = get_log_file_path();
        if (log_file_path == NULL) {
            free(last_line);
            return;
        }

        FILE *file = fopen(log_file_path, "r");
        if (file == NULL) {
            printf("Error opening log.txt\n");
            free(last_line);
            free(log_file_path);
            return;
        }

        int line_count = 0;
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n') {
                line_count++;
            }
        }
        fclose(file);

        if (line_count >= MAX_LOG) {
            file = fopen(log_file_path, "r");
            if (file == NULL) {
                printf("Error opening log.txt\n");
                free(last_line);
                free(log_file_path);
                return;
            }

            FILE *temp_file = fopen("temp.txt", "w");
            if (temp_file == NULL) {
                printf("Error opening temp.txt\n");
                fclose(file);
                free(last_line);
                free(log_file_path);
                return;
            }

            char *temp = (char *) malloc(LOG_LINE_MAX * sizeof(char));
            int current_line = 0;

            while (fgets(temp, LOG_LINE_MAX, file) != NULL) {
                if (current_line > 0) {
                    fprintf(temp_file, "%s", temp);
                }
                current_line++;
            }

            free(temp);
            fclose(file);
            fclose(temp_file);

            remove(log_file_path);
            rename("temp.txt", log_file_path);
        }

        FILE *fp = fopen(log_file_path, "a");
        free(log_file_path);
        if (fp == NULL) {
            printf("Error opening log.txt\n");
            free(last_line);
            return;
        }

        if (command[strlen(command) - 1] == '\n') {
            command[strlen(command) - 1] = '\0';
        }
        fprintf(fp, "%s\n", command);
        fclose(fp);
    }

    free(last_line);
}

// Log purge: Clears all logs
void log_command_purge() {
    char *log_file_path = get_log_file_path();
    if (log_file_path == NULL) {
        return;
    }

    FILE *file = fopen(log_file_path, "w");
    free(log_file_path);
    if (file != NULL) {
        fclose(file);
    } else {
        printf("Error purging log.\n");
    }
}

// Log execute: Execute a command from the log
void log_command_execute(int index) {
    char *log_file_path = get_log_file_path();
    if (log_file_path == NULL) {
        return;
    }

    FILE *file = fopen(log_file_path, "r");
    free(log_file_path);
    if (file == NULL) {
        printf("Error opening log file.\n");
        return;
    }

    char command[LOG_LINE_MAX];
    int current_index = 1;
    while (fgets(command, LOG_LINE_MAX, file) != NULL) {
        if (current_index == index) {
            printf("Executing command: %s", command);
            input_utils(command);
            break;
        }
        current_index++;
    }

    if (current_index != index) {
        printf("No command at index %d\n", index);
    }

    fclose(file);
}

// Function to handle log commands
void log_command(char* args[]) {
    if (args[1] == NULL) {
        FILE *file = fopen(get_log_file_path(), "r");
        if (file == NULL) {
            printf("Error opening log file.\n");
            return;
        }

        char command[LOG_LINE_MAX];
        int current_index = 0;
        while (fgets(command, LOG_LINE_MAX, file) != NULL) {
            printf("%s",  command);
            current_index++;
        }

        fclose(file);
        return;
    }

    if (strcmp(args[1], "purge") == 0) {
        log_command_purge();
    } else if (strcmp(args[1], "execute") == 0 && args[2] != NULL) {
        int index = atoi(args[2]);
        log_command_execute(index);
    } else {
        printf("Unknown log command.\n");
    }
}