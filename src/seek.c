#include "header_library.h"
#include "utils.h"

int found_match = 0; // Flag to track if any match is found
int only_one_match = 0; // Track if only one match is found for the -e flag
char only_match_path[PATH_SIZE];


void color_print(const char *path, int is_dir) {
    if (is_dir)
        printf(BLUE"%s\n"RESET, path); // Blue for directories
    else
        printf("%s\n", path); // Normal for files
}

int has_permission(const char *path, int is_dir) {
    if (is_dir)
        return access(path, X_OK) == 0;
    else
        return access(path, R_OK) == 0;
}

void dfs(char rel_dir[], char abs_path[], char search_file[], int flag_id) {
    DIR *dir = opendir(abs_path);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char next_rel[PATH_SIZE], next_abs[PATH_SIZE];
        snprintf(next_rel, PATH_SIZE, "%s/%s", rel_dir, entry->d_name);
        snprintf(next_abs, PATH_SIZE, "%s/%s", abs_path, entry->d_name);

        struct stat st;
        if (stat(next_abs, &st) == -1) continue;

        int is_dir = S_ISDIR(st.st_mode);
        int is_file = S_ISREG(st.st_mode);

        int match = strncmp(entry->d_name, search_file, strlen(search_file)) == 0;
        if (match) {
            if (flag_id == 1 && is_dir) { // -d
                found_match = 1;
                if (only_one_match == 0) {
                    only_one_match = 1;
                    strncpy(only_match_path, next_abs, PATH_SIZE);
                } else {
                    only_one_match = -1;
                }
                color_print(next_rel, 1);
            } else if (flag_id == 2 && is_file) { // -f
                found_match = 1;
                if (only_one_match == 0) {
                    only_one_match = 1;
                    strncpy(only_match_path, next_abs, PATH_SIZE);
                } else {
                    only_one_match = -1;
                }
                color_print(next_rel, 0);
            } else if (flag_id == 0) { // no flag
                found_match = 1;
                color_print(next_rel, is_dir);
            }
        }

        if (is_dir)
            dfs(next_rel, next_abs, search_file, flag_id);
    }
    closedir(dir);
}

void expand_path(const char *input, char *output) {
    if (input[0] == '~') {
        const char *home = getenv("HOME");
        if (!home) home = getpwuid(getuid())->pw_dir;
        snprintf(output, PATH_SIZE, "%s%s", home, input + 1);
    } else {
        realpath(input, output);
    }
}

void seek(char* args[]) {
    int flag_d = 0, flag_f = 0, flag_e = 0;
    int arg_idx = 1;
    while (args[arg_idx] && args[arg_idx][0] == '-') {
        if (strcmp(args[arg_idx], "-d") == 0) flag_d = 1;
        else if (strcmp(args[arg_idx], "-f") == 0) flag_f = 1;
        else if (strcmp(args[arg_idx], "-e") == 0) flag_e = 1;
        else {
            fprintf(stderr, "Invalid flags!\n");
            return;
        }
        arg_idx++;
    }
    if (flag_d && flag_f) {
        fprintf(stderr, "Invalid flags!\n");
        return;
    }
    if (!args[arg_idx]) {
        fprintf(stderr, "Usage: seek <flags> <search> <target_directory>\n");
        return;
    }
    char search_file[PATH_SIZE];
    strncpy(search_file, args[arg_idx], PATH_SIZE);
    arg_idx++;
    char target_dir[PATH_SIZE];
    if (args[arg_idx]) {
        expand_path(args[arg_idx], target_dir);
    } else {
        getcwd(target_dir, PATH_SIZE);
    }

    char rel_dir[PATH_SIZE] = ".";
    found_match = 0;
    only_one_match = 0;
    only_match_path[0] = '\0';

    int flag_id = 0;
    if (flag_d) flag_id = 1;
    else if (flag_f) flag_id = 2;

    dfs(rel_dir, target_dir, search_file, flag_id);

    if (!found_match) {
        printf("No match found!\n");
        return;
    }

    if (flag_e && only_one_match == 1) {
        struct stat st;
        if (stat(only_match_path, &st) == -1) {
            printf("Missing permissions for task!\n");
            return;
        }
        int is_dir = S_ISDIR(st.st_mode);
        int is_file = S_ISREG(st.st_mode);
        if (is_dir && has_permission(only_match_path, 1)) {
            printf("%s\n", only_match_path);
            if (chdir(only_match_path) != 0)
                printf("Missing permissions for task!\n");
        } else if (is_file && has_permission(only_match_path, 0)) {
            printf("%s\n", only_match_path);
            FILE *fp = fopen(only_match_path, "r");
            if (fp) {
                char buf[PATH_SIZE];
                while (fgets(buf, PATH_SIZE, fp))
                    printf("%s", buf);
                printf("\n");
                fclose(fp);
            }
        } else {
            printf("Missing permissions for task!\n");
        }
    }
}