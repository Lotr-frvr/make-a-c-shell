#include <stdio.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <libgen.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define ERROR_COLOR "\033[1;31m"
#define RESET "\033[0m"
// prompt.c
#define PATH_SIZE 512 // hop directory commands
#define USER_NAME_SIZE 256
#define HOST_NAME_SIZE 256

// log # commands
#define MAX_LOG 15


//log_write_to_file
#define LOG_LINE_MAX 1024
#define LOG_FILENAME "log.txt"
// input.c
#define ARGS_COMMAND 128
//reveal.c
#define MAX_FILES 128


// system commands
// fg.c bg.c
#define CURRFG_SIZE 256
#define BG_MAX 128

//seek.c
#define FILE_SEARCH 128

// proclore.c
#define PATH_NAME 256
#define BUFFER_SIZE 256

// system.c

//pipe.c
#define MAX_PIPES 128

//redirect.c
#define MAX_REDIRECTS 128

//activities.c
#define PROCESS_LIST_NUM 128


