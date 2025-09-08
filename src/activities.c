#include "header_library.h"
#include "utils.h"

struct procinfo {
    pid_t pid;
    char name[CURRFG_SIZE];
    char state[16];
};

int compare_procinfo(const void *a, const void *b) {
    return strcmp(((struct procinfo*)a)->name, ((struct procinfo*)b)->name);
}

void activities() {
    struct procinfo list[MAX_BG_PROCS];
    int count = 0;

    for (int i = 0; i < bgi; i++) {
        if (bgs[i].pid <= 0) continue;
        list[count].pid = bgs[i].pid;
        strcpy(list[count].name, bgs[i].name);

        // Check process state
        char proc_path[64];
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", bgs[i].pid);
        FILE *fp = fopen(proc_path, "r");
        if (fp) {
            char buf[512];
            if (fgets(buf, sizeof(buf), fp)) {
                char state;
                sscanf(buf, "%*d %*s %c", &state);
                if (state == 'T') // stopped
                    strcpy(list[count].state, "Stopped");
                else
                    strcpy(list[count].state, "Running");
            } else {
                strcpy(list[count].state, "Stopped");
            }
            fclose(fp);
        } else {
            strcpy(list[count].state, "Stopped");
        }
        count++;
    }

    // Sort lexicographically by command name
    qsort(list, count, sizeof(struct procinfo), compare_procinfo);

    // Print
    for (int i = 0; i < count; i++) {
        printf("%d : %s - %s\n", list[i].pid, list[i].name, list[i].state);
    }
}

