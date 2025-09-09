#include "header_library.h"
#include "utils.h"

char home_path[PATH_SIZE];
char prev_path[PATH_SIZE];

int currfgid=0;
int currfgtime=0;
char currfgcom[CURRFG_SIZE];



int bgi=0;
struct backproc *bgs=NULL;


int main(){
    getcwd(home_path, sizeof(home_path));
    getcwd(prev_path, sizeof(prev_path)); //initialize both paths to the home directory first
    signal(SIGCHLD, bg_exit_handler); // Background processes exiting
    signal(SIGINT, ctrlc);    // Ctrl-C
    signal(SIGTSTP, ctrlz);   // Ctrl-Z

    while(1){
         prompt(home_path);
         input();
    }

    return 0;
}