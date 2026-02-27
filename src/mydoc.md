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

`Issue-> Background processes started by the shell do not persist across new terminal sessions. When you close and reopen the terminal, the shell does not retain information about background jobs from previous sessions. Only processes started in the current session are tracked and managed.
`
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

### seek.c

Recursively searches for files and/or directories matching the search term in the target directory. Uses dfs
Prints relative paths of all matches, coloring files in green and directories in blue.
Supports flags to filter results:
-d: Only directories.
-f: Only files.
-e: If exactly one match is found, prints file content (if file) or changes to the directory (if directory), checking permissions.
Handles errors for invalid flag combinations (e.g., -d and -f together) and missing matches.

Permission Checks: Uses `access` to check read/execute permissions for files and directories.

### pipes, i/o redirection and redirection along pipes
Redirect 

Shell redirects by opening the target file, then replacing the standard descriptor (stdin/stdout) in the child process with the opened file descriptor using dup2().

> : open with O_WRONLY|O_CREAT|O_TRUNC — overwrite.

>>: open with O_WRONLY|O_CREAT|O_APPEND — append.

< : open with O_RDONLY — used as standard input.

After dup2() the child calls execvp(); the parent waits for the child.

Pipes 

pipe() gives two descriptors: read (p[0]) and write (p[1]).

For a pipeline cmd1 | cmd2 | cmd3, the shell connects cmd1's stdout to a pipe, cmd2 reads from that pipe and writes to the next pipe, etc. Each stage is typically a separate fork()+exec.

Parent must carefully close unused ends of pipes and wait for children.

Background jobs

For background jobs (&), the child typically calls setpgid(0, 0) so it runs in its own process group; shell does not block waiting for the job, but must track it for job control.

Issue -> echo "hello world" > out.txt may pass the literal argument "hello world" (including quotes) to the executed program or to a file write, producing output that contains the quotes instead of hello world.
You can pass hello world without quotes


### activities

prints a list of all the processes currently running that were spawned by your shell in lexicographic order. This list contains the following information about all processes :

    Command Name

    pid

    state : running or stopped

Format of an entry is:

[pid] : [command name] - [State]

### signals 

Take the pid of a process and send a signal to it which corresponds to the signal number (which is provided as an argument). Print error “No such process found”, if process with given pid does not exist. Takes signal number’s modulo with 32 before checking which signal it belongs to (assuming x86/ARM machine). 

Ctrl - C

Interrupts any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

Ctrl - D

Logs out of your shell (after killing all processes) while having no effect on the actual terminal.

Ctrl - Z

Pushes the (if any) running foreground process to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.

A succesful run with ctrlz --
``` 
JohnDoe@SYS:~$ ./a.out 
JohnDoe@SYS:~$ sleep 100 
^ZStopped process 530688 with SIGTSTP

Process moved to background (Stopped)

Moved process 0 to background (Stopped)
JohnDoe@SYS:~ sleep : 3s $ activities
530688 : sleep - Stopped
JohnDoe@SYS:~$ ping 530688 18
Sent signal 18 to process with pid 530688
JohnDoe@SYS:~$ activities
530688 : sleep - Running
JohnDoe@SYS:~$ exit 
```
### fg and bg 

fg <pid>

Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal. Print “No such process found”, if no process with given pid exists.

bg <pid>

Changes the state of a stopped background process to running (in the background). If a process with given pid does not exist, print “No such process found” to the terminal.

A sample run 
```
JohnDoe@SYS:~$ activities
No background processes

JohnDoe@SYS:~$ sleep 100
^Z
Stopped process 12345 with SIGTSTP
Process moved to background (Stopped)

JohnDoe@SYS:~$ activities
12345 : sleep - Stopped

JohnDoe@SYS:~$ bg 12345
Process with PID 12345 moved to background.
Process 12345 running in background.

JohnDoe@SYS:~$ activities
12345 : sleep - Running

JohnDoe@SYS:~$ sleep 200
^Z
Stopped process 12346 with SIGTSTP
Process moved to background (Stopped)

JohnDoe@SYS:~$ activities
12345 : sleep - Running
12346 : sleep - Stopped

JohnDoe@SYS:~$ fg 12346
Process 12346 continued in foreground.
^C
Interrupted process 12346 with SIGINT

JohnDoe@SYS:~$ activities
12345 : sleep - Running

JohnDoe@SYS:~$ fg 12345
Process 12345 continued in foreground.
^C
Interrupted process 12345 with SIGINT

JohnDoe@SYS:~$ activities
No background processes
```

### neonate 
The neonate command continuously prints the PID (Process ID) of the most recently created process on the system at regular time intervals.It keeps running until the user presses the x key, at which point it terminates.This feature simulates a "neonate monitor" for processes — always showing the "newborn" process on the system.

Usage:

`neonate -n [time_arg]`
where:

`time_arg` is an integer representing the number of seconds between updates.

The command will:

Fetch the PID of the most recently created process on the system.Print that PID to the terminal.Sleep for `time_arg` seconds.

Repeat until the user presses x.
Issue :->

echo hello world : quotes arent handles 

log -> temp.txt : shows warning 
