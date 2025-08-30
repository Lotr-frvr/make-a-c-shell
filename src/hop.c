#include "header_library.h"
#include "utils.h"
    
extern  char home_path[PATH_SIZE];
extern  char prev_path[PATH_SIZE];


void hop(char* args[]) {
    char curr_directory[PATH_SIZE];
    
    if (args[1] == NULL) {
        // No argument: go to the home_path directory
        getcwd(prev_path, sizeof(prev_path));
        chdir(home_path);
        printf("%s\n", home_path);
        return;
    }

    // Handle ".", "..", "~", and "-"
    int i = 1;
    while (args[i]) {
        getcwd(curr_directory, sizeof(curr_directory));

        if (strcmp(args[i], "~") == 0) {
            // Go to home_path directory
            getcwd(prev_path, sizeof(prev_path));
            chdir(home_path);
            printf("%s\n", home_path);
        } 
        else if(strstr(args[i],"~/")!=NULL){
            char path[PATH_SIZE];
            strcpy(path, home_path);
            strcat(path, "/");
            strcat(path, args[i]+2);
            struct stat sb;
            if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                getcwd(prev_path, sizeof(prev_path));
                chdir(path);
                printf("%s\n", getcwd(curr_directory, sizeof(curr_directory)));
            } else {
                printf("No such directory: %s\n", path);
            }
        }
        else if (strcmp(args[i], "-") == 0) {
            // Go to the previous directory
            chdir(prev_path);
            strcpy(prev_path, curr_directory);
            printf("%s\n", getcwd(curr_directory, sizeof(curr_directory)));
        } 
        else if (strcmp(args[i], ".") == 0) {
            // Print current directory
            getcwd(prev_path, sizeof(prev_path));
            printf("%s\n", curr_directory);
        } 
        else if (strcmp(args[i], "..") == 0) {
            // Go to the parent directory
            if (strcmp(curr_directory, home_path) == 0) {
                printf("You are already in the home_path directory\n");
            } else {
                strcpy(prev_path, curr_directory);
                chdir("..");
                printf("%s\n", getcwd(curr_directory, sizeof(curr_directory)));
            }
        } 
        else {
            // Go to the specified directory (absolute or relative path)
            struct stat sb;
            if (args[i][0] == '/') {
                // Absolute path
                if ((stat(args[i], &sb) == 0 )&& S_ISDIR(sb.st_mode)) {
                    getcwd(prev_path, sizeof(prev_path));
                    chdir(args[i]);
                    printf("%s\n", getcwd(curr_directory, sizeof(curr_directory)));
                } else {
                    printf("No such directory: %s\n", args[i]);
                }
            } 
            else {
                // Relative path
                char path[PATH_SIZE];
                strcpy(path, curr_directory);
                strcat(path, "/");
                strcat(path, args[i]);

                if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    getcwd(prev_path, sizeof(prev_path));
                    chdir(path);
                    printf("%s\n", getcwd(curr_directory, sizeof(curr_directory)));
                } else {
                    printf("No such directory: %s\n", path);
                }
            }
        }

        i++;
    }
    return;
}