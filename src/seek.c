#include "header_library.h"
#include "utils.h"

void seek(char* args[]){
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: seek <pid>\n");
        return;
    }

    pid_t pid = atoi(args[1]);
    print_Process_info(pid);
}