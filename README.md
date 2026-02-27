# Make-a-C-Shell

A custom Unix-like shell implemented in C, supporting advanced job control, custom commands, piping, redirection, and process management. This shell mimics many features of Bash, with additional built-in utilities and robust error handling.

---

## Features

- **Prompt**: Displays user and host information, current directory, and last foreground process duration (if >2s).
- **Input Parsing**: Handles multiple commands separated by `;` or `&`, supports tokenization and whitespace management.
- **Custom Commands**:
  - `hop`: Enhanced `cd` supporting `.`, `..`, `~`, `-`, and multiple sequential arguments.
  - `reveal`: Advanced `ls` with `-a` (show hidden), `-l` (detailed), color coding, and robust path handling.
  - `log`: Persistent command history (max 15), with `log purge` to clear and `log execute <index>` to re-run.
  - `seek`: Recursive search for files/directories with `-d`, `-f`, `-e` flags, permission checks, and color output.
  - `proclore`: Displays process info (PID, status, group, memory, executable path) for a given PID.
  - `activities`: Lists all background processes spawned by the shell, with PID, command, and state.
  - `fg <pid>` / `bg <pid>`: Foreground/background job control for stopped/running processes.
  - `neonate -n <seconds>`: Monitors and prints the PID of the most recently created process at intervals, exits on `x` key.
- **System Commands**: Executes any valid Bash command in foreground or background, with job tracking.
- **Pipes & Redirection**: Supports `|`, `>`, `>>`, `<` for chaining commands and redirecting input/output.
- **Signal Handling**: Implements Ctrl-C (SIGINT), Ctrl-Z (SIGTSTP), Ctrl-D (logout), and custom signal sending via `ping <pid> <signal>`.
- **Robust Error Handling**: Graceful handling of invalid commands, memory allocation failures, and file/permission errors.

---

## Usage

1. **Build the Shell**
	```sh
	cd src
	make
	./a.out
	```

2. **Prompt Example**
	```
    username@hostname:current_directory$
	```

3. **Command Examples**
	- Change directory: `hop .. ~/Documents -`
	- List files: `reveal -al ~/Downloads`
	- Show command history: `log`
	- Purge history: `log purge`
	- Execute from history: `log execute 3`
	- Search recursively: `seek -f -e myfile.txt`
	- Show process info: `proclore 1234`
	- List jobs: `activities`
	- Move job to foreground: `fg 1234`
	- Move job to background: `bg 1234`
	- Monitor new processes: `neonate -n 2`
	- Send signal: `ping 1234 18`
	- Run with pipes/redirection: `ls -l | grep txt > out.txt`

---

## Implementation Highlights

- **Prompt**: Uses `getcwd`, `getlogin_r`, and `gethostname` for dynamic prompt info.
- **Input Handling**: Uses `getline` and `strtok` for robust command parsing.
- **Directory Navigation**: `hop` uses `chdir` and `stat` for flexible navigation.
- **File Listing**: `reveal` uses `opendir`, `readdir`, `stat`, and color codes output.
- **Logging**: Maintains a persistent log in the user's home directory, skips duplicates and log commands.
- **Job Control**: Tracks background/foreground jobs using `fork`, `waitpid`, `setpgid`, and custom structs.
- **Process Info**: `proclore` reads `/proc/<pid>/stat` and `/proc/<pid>/exe` for detailed process data.
- **Recursive Search**: `seek` implements DFS with permission checks and color output.
- **Pipes/Redirection**: Uses `dup2`, `pipe`, and careful file descriptor management.
- **Signal Handling**: Handles signals for job control and custom signal sending.

---

## Directory Structure

```
src/
  main.c           # Shell entry point
  prompt.c         # Prompt display logic
  input.c          # Input parsing and command dispatch
  hop.c            # Directory navigation
  reveal.c         # File listing
  log.c            # Command history
  seek.c           # Recursive search
  proclore.c       # Process info
  activities.c     # Job listing
  fg.c, bg.c       # Foreground/background job control
  neonate.c        # Neonate monitor
  pipes.c          # Pipe handling
  redirect.c       # I/O redirection
  utils.h, header_library.h # Shared headers
  Makefile         # Build instructions
  ...
```

---
