#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/utsname.h>

char *get_path(char * command);
//char **split_string(char *buf ,char **argv , int len);
char **split_string(char* input, char ** argv);
void exec(char **argv);
