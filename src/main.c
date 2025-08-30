#include "header_library.h"
#include "utils.h"

char home_path[PATH_SIZE];
char prev_path[PATH_SIZE];

int main(){
    getcwd(home_path, sizeof(home_path));
    getcwd(prev_path, sizeof(prev_path)); //initialize both paths to the home directory first
    while(1){
         prompt(home_path);
         input();
    }

    return 0;
}