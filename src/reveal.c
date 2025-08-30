#include "header_library.h"
#include "utils.h"

// Function to display file info with colors based on file type
void display_file_info(char *file_name, char *file_path, int show_hidden, int show_long) {
    struct stat fileStat;
    char full_path[PATH_SIZE];
    snprintf(full_path, sizeof(full_path), "%s/%s", file_path, file_name);
    
    // Skip hidden files if '-a' is not specified
    if (!show_hidden && file_name[0] == '.') {
        return;
    }
    
    if (stat(full_path, &fileStat) < 0) {
        perror("stat");
        return;
    }
    
    // Determine color for the file type
   const char *color;
    if (S_ISDIR(fileStat.st_mode)) {
        color = BLUE;  // Directories in blue
    } else if (file_name[0] == '.') { 
        color = BLUE;  // Hidden files in blue (you can change this to another color if needed)
    } else if (fileStat.st_mode & S_IXUSR || fileStat.st_mode & S_IXGRP || fileStat.st_mode & S_IXOTH) {
        color = GREEN;  // Executable files in green
    } else {
        color = RESET;  // Regular files in default color (white)
    }


    if (show_long) {
        // Display file permissions
        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

        // Display number of links
        printf(" %ld", (long)fileStat.st_nlink);

        // Display owner and group
        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group *gr = getgrgid(fileStat.st_gid);
        printf(" %s %s", pw->pw_name, gr->gr_name);

        // Display file size
        printf(" %4ld", (long)fileStat.st_size);

        // Display last modification time
        char timebuf[80];
        struct tm lt;
        localtime_r(&fileStat.st_mtime, &lt);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M ", &lt);
        printf(" %s", timebuf);

        // Display file name with color
        // execuatble files in green
       printf("%s%s%s\n", color, file_name, RESET);
        
       
    } else {
        // Display file name with color
        printf("%s%s%s\t", color, file_name, RESET);
    }
}

void reveal(char *args[]) {
    struct dirent *dp;
    int flag_id = 0;
    char curr_directory[PATH_SIZE];
    getcwd(curr_directory, sizeof(curr_directory));
    extern char home_path[PATH_SIZE];
    extern char prev_path[PATH_SIZE];
    char file_path[PATH_SIZE];
    strcpy(file_path, curr_directory);  // default path is the current directory

    // Process arguments and flags
    int i = 1;
    while (args[i]) {
        if (strstr(args[i], "-") == args[i]) {
            for (int j = 1; j < strlen(args[i]); j++) {
                if (args[i][j] == 'a') {
                    flag_id |= 2;
                } else if (args[i][j] == 'l') {
                    flag_id |= 1;
                }
            }
        } else {
            // Handle path symbols like ".", "..", "~", "-"
            if (strcmp(args[i], ".") == 0) {
                // Already in current directory, no change needed
            } else if (strcmp(args[i], "..") == 0) {
                strcpy(file_path, dirname(curr_directory));
            } else if (strcmp(args[i], "~") == 0) {
                strcpy(file_path, home_path);
            } else if (strcmp(args[i], "-") == 0) {
                strcpy(file_path, prev_path);
            } else if (strstr(args[i], "~/") == args[i]) {
                strcpy(file_path, home_path);
                strcat(file_path, args[i] + 1);
            } else {
                strcpy(file_path, args[i]);
            }
        }
        i++;
    }

    // Open the directory
    DIR *dir = opendir(file_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Collect all files in the directory
    char files[MAX_FILES][PATH_SIZE];
    int num_files = 0;
    while ((dp = readdir(dir)) != NULL) {
        strcpy(files[num_files++], dp->d_name);
    }
    closedir(dir);

    // Sort the files lexicographically
    for (int i = 0; i < num_files - 1; i++) {
        for (int j = i + 1; j < num_files; j++) {
            if (strcmp(files[i], files[j]) > 0) {
                char temp[PATH_SIZE];
                strcpy(temp, files[i]);
                strcpy(files[i], files[j]);
                strcpy(files[j], temp);
            }
        }
    }

    // Display files based on flags
    int show_hidden = flag_id & 2;
    int show_long = flag_id & 1;
    for (int i = 0; i < num_files; i++) {
        display_file_info(files[i], file_path, show_hidden, show_long);
    }
    if(!show_long){
        printf("\n");
    }
}