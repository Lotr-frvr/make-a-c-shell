### prompt.c

in hp@hp-pavillion -> hp is the user_name and hp-pavillion is the host name
`getcwd` return the current working directory
`getlogin_r` retrieves the name of the user logged in on the controlling terminal.
`gethostname` retrieves the name of the current host (computer).

### input.c

`getline(&input_command, &len, stdin)` reads a line from the user, storing it in input_command and updating len with the buffer size.
If it `returns -1`, it means an error occurred or end-of-file (EOF) was reached.
The condition != -1 means "if a line was read successfully."

`strtok` tokenizes the input. `;` and `&` handled.
spaces in between handled

### hop.c

Implemented the “.”, “..”, “~”, and “-” flags in hop. ~ represents the home directory of shell. Supports both absolute and relative paths, along with paths from home directory.
If more than one argument is present, execute hop sequentially with all of them being the argument one by one (from left to right).
If no argument is present, then hop into the home directory.

`chdir`- chnages the current working dir
`struct stat`- checks file/directory status    
### reveal.c

supports flags for showing hidden files (-a) and displaying detailed file information (-l). The command can handle special path arguments like ., .., ~, -, and ~/subdir, allowing flexible navigation and listing from various locations. Files are displayed with color coding based on their type (directory, executable, hidden), and the output is sorted lexicographically. Exception cases handled include skipping hidden files unless -a is specified, printing errors if a directory or file cannot be accessed, and handling invalid paths gracefully.

any file name beginning with `.` is a hidden file 
any folder name beginning with `.` is a hidden folder 
you can see them when you do reaveal -a els eyou wont see ., .. and hidden files/folders

`struct stat` -> get the file metadata 
S_ISDIR(fileStat.st_mode): Checks if the file is a directory.
S_IXUSR, S_IXGRP, S_IXOTH: Check if the file is executable by user, group, or others.
S_IRUSR, S_IWUSR, S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH: Check read/write permissions for user, group, and others.
These macros are defined in <sys/stat.h> and are used to interpret the st_mode field of struct stat for file type and permissions.

dirname /usr/bin/
              -> "/usr"

       dirname dir1/str dir2/str
              -> "dir1" followed by "dir2"

       dirname stdio.h
              -> "."

`opendir`, `readdir`, and `closedir` for directory operations
The number of hard links `fileStat.st_nlink`
The owner and group names `getpwuid, getgrgid`
The file size in bytes `fileStat.st_size`


### log.c

log

Implemented a ‘log’ command which is similar to the actual history command in bash. 

#define MAX_LOG 15
 
 You must overwrite the oldest commands if more than the set number of commands are entered. You should track the commands across all sessions and not just one. The commands should be printed from oldest to recent moving top to down (Refer example below).

    Haven't stored the log command in log. In cases where multiple commands separated by ; or & contain a log command,hasn't stored the entire command string. 
    
log purge
Clears all the log currently stored. Do not store this command in the log.

log execute <index>

Execute the command at position in log (ordered from most recent to oldest).

Checks for memory allocation failures when creating file paths.
Handles errors when opening, creating, or writing to the log file.
Skips logging duplicate commands (does not log if the last command is the same).
Skips logging commands containing "log" to avoid recursive logging.
Handles cases where the log file does not exist and creates it if needed.
Handles purging errors and missing log entries for execution by index.

`checkandcreatefile` The log file is created in the user's home directory if it does not exist


### system commands

The shell was implemented to execute other system commands present in Bash, such as emacs, gedit, etc., in both foreground and background processes.

Foreground Process
- Executing a command in the foreground meant the shell waited for that process to complete and regained control afterwards. Control of the terminal was handed over to this process while it was running.
- If a foreground process took more than 2 seconds to run, the time taken and the name of the process were printed in the next prompt, with the time rounded down to the nearest integer.

Background Process
- Any command invoked with “&” was treated as a background command. The shell spawned the process but did not hand control of the terminal to it, allowing the shell to continue accepting other user commands. Whenever a new background process was started, the PID of the newly created background process was printed on the shell.



- Background processes were not handled for custom commands (hop, reveal, log, etc.).
- Multiple background processes could be run simultaneously.
- When a background process finished, a message was displayed to the user autonomously, without further interaction required.
- The process name and PID were printed when a background process ended, along with an indication of whether the process ended normally or abnormally.

The shell uses `fork` to create child processes for external commands. Foreground processes are managed with `waitpid`, while background processes run independently.

`setpgid` on the condition that it is a background process--> this helps wiht proclore 
backround processes will have a different group
note :-
for sleep 4; sleep 3;

in the next prompt it will only show `~ sleep 3 $`

defined the struct backproc in headerfiel. Initialised it in main.c and called it in input.c using extern

gedit works, sleep works
cd doesn't work because 
The cd command is a shell built-in and cannot be executed using execvp as an external command.

ls & pwd; echo hi & sleep 1
here `&` and `;` seperates the different commands 
& indicates bg processes 

### proclore.c

Information required to print :
- pid
- Process Status (R/R+/S/S+/Z)
- Process group
- Virtual Memory
- Executable path of process

create file paths for accessing the process information in the proc filesystem for a gvien process ID

snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid); // path to the executable
llly two more paths to stat file, virtual memory

It constructs paths to `/proc/<pid>/stat (process status)`, `/proc/<pid>/exe` (executable path).
It opens `/proc/<pid>/stat` and reads several fields:
`pid`: Process ID.
`comm[BUFFER_SIZE]`: The command name (executable name) of the process.
`state`: The process state (e.g., 'R' for running, 'S' for sleeping).
`pgrp`: Process group ID.

Compare the process group ID (pgrp) with your shell’s foreground process group (getpgrp()).
If they match, append + to the status character.

Other fields are skipped until the 23rd field, which is vsize (virtual memory size in bytes).
It uses readlink on `/proc/<pid>/exe` to get the full path to the process's executable.
Finally, it prints the PID, process status, process group, virtual memory (in KB), and executable path.

`readlink` is a system call in C that reads the value of a symbolic link (symlink). It copies the path that the symlink points to into a buffer.
`ssize_t len = readlink("/proc/1234/exe", buf, bufsize);`

