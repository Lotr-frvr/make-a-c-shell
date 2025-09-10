#include "header_library.h"
#include "utils.h"
int get_latest_pid() {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("opendir");
        return -1;
    }
    struct dirent *entry;
    int max_pid = -1;
    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            if (pid > max_pid) max_pid = pid;
        }
    }
    closedir(dir);
    return max_pid;
}

void set_noncanonical_mode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
}

void neonate(char* args[]) {
    if (args[1] == NULL || strcmp(args[1], "-n") != 0 || args[2] == NULL) {
        printf("Usage: neonate -n <time_arg>\n");
        return;
    }

    int interval = atoi(args[2]);
    if (interval <= 0) {
        printf("Invalid time_arg\n");
        return;
    }

    set_noncanonical_mode(1);  // enable raw input

    while (1) {
        int latest_pid = get_latest_pid();
        if (latest_pid != -1) {
            printf("%d\n", latest_pid);
            fflush(stdout);
        }

        for (int i = 0; i < interval * 10; i++) { // check every 0.1s
            usleep(100000);
            char c;
            if (read(STDIN_FILENO, &c, 1) > 0) {
                if (c == 'x') {
                    set_noncanonical_mode(0);
                    return;
                }
            }
        }
    }

    set_noncanonical_mode(0); // restore normal input mode
}
