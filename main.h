//Permutation of all libraries to this file so that project easier to read

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
char **split_string(char* input, char ** argv ,int *argc);
void exec(char **argv);
int additional_functions(char** argv);
char ** replace_variables(char **argv,int argc);
int takvim ();
int file_info ();
int hesapla();