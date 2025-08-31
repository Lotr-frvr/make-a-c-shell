#include "header_library.h"
#include "utils.h"
extern int currfgid;
extern int currfgtime;
extern char currfgcom[CURRFG_SIZE];


void prompt(char oghome_path[]){
    char user_name[USER_NAME_SIZE];
    char host_name[HOST_NAME_SIZE];
    
    getlogin_r(user_name, sizeof(user_name)); //
    gethostname(host_name, sizeof(host_name));

    char curr_directory[PATH_SIZE];
    getcwd(curr_directory, sizeof(curr_directory));

    char prompt_path[PATH_SIZE];
    // we have to remove the oghome_path from teh curr_directory
    // the current directory should be relative to the oghome_path
    if (strncmp(curr_directory, oghome_path, strlen(oghome_path)) == 0) {
        // The current directory is within the home directory
        if (strlen(curr_directory) == strlen(oghome_path)) {
            // Current directory is exactly the home directory
            strcpy(prompt_path, "~");
        } else {
            // Current directory is a subdirectory of the home directory
            snprintf(prompt_path, sizeof(prompt_path), "~%s", curr_directory + strlen(oghome_path));
        }
    } else {
        // The current directory is outside the home directory
        strncpy(prompt_path, curr_directory, sizeof(prompt_path));// this prints the absolute path
    }
    printf(GREEN "%s@%s" RESET, user_name, host_name);
    printf(":");
    printf(BLUE "%s" RESET, prompt_path);
    if(currfgtime>2){
        printf(" %s : %ds ", currfgcom, currfgtime);
        currfgtime = 0;
        currfgcom[0] = '\0';
    }
    printf("$ ");
    return ;
}