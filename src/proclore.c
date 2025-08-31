#include "header_library.h"
#include "utils.h"

void print_Process_info(pid_t pid) {
    // Implementation of process information printing
    char exec_path[PATH_NAME];
    char stat_path[PATH_NAME];
    char virtual_mem_path[PATH_NAME];

    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
    snprintf(exec_path, sizeof(exec_path), "/proc/%d/exe", pid);
    snprintf(virtual_mem_path, sizeof(virtual_mem_path), "/proc/%d/maps", pid);

    /*---Process Status, Groups, and Virtual Memory---*/
    FILE *stat_file = fopen(stat_path, "r");
    if (stat_file == NULL) {
        perror("Error opening stat file");
        return;
    }
    // Data from /proc/[pid]/stat (fields we're interested in)
    int ppid, pgrp, session, tty_nr, tpgid;
    char comm[BUFFER_SIZE], state;
    unsigned long vsize = 0;
    // Read up to the 23rd field (vsize)
    fscanf(stat_file, "%d %s %c %d %d %d %d %d", &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid);
    // Skip fields 8-22
    for (int i = 0; i < 15; i++) fscanf(stat_file, "%*s");
    fscanf(stat_file, "%lu", &vsize); // 23rd field: virtual memory size in bytes
    fclose(stat_file);

    // ********** Executable Path **********
    char resolved_exec_path[BUFFER_SIZE];
    ssize_t len = readlink(exec_path, resolved_exec_path, sizeof(resolved_exec_path) - 1);
    if (len != -1) {
        resolved_exec_path[len] = '\0'; // Null-terminate the path
    } else {
        strcpy(resolved_exec_path, "Executable path not found");
    }

    // ********** Print Process Information **********
    printf("PID: %d\n", pid);
    if(pgrp == getpgrp()){
        printf("Process Status: %c+\n", state);//Foreground process
    }
    else
        printf("Process Status: %c\n", state);//Background process

    printf("Process Group: %d\n", pgrp);
    printf("Virtual Memory: %lu KB\n", vsize / 1024); // Convert bytes to KB
    printf("Executable Path: %s\n", resolved_exec_path);

}


void proclore(char* args[]) {
    pid_t pid;
    if (args[1] == NULL) {
        pid = getpid();
        print_Process_info(pid);
        return;
    } else {
        pid = atoi(args[1]);
    }
    print_Process_info(pid);
    return;
}