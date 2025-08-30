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

The number of hard links `fileStat.st_nlink`
The owner and group names `getpwuid, getgrgid`
The file size in bytes `fileStat.st_size`