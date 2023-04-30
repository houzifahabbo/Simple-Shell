/*************************************************************************************
 *
 *    File: main.h
 * Project: system-programing-project-1
 * Authors: Hozaifah Habbo, Ola Helany, Nour Chami, Muslim Umalatov
 * Purpose: Permutation of all libraries to this file so that project is easy to read
 *
 *************************************************************************************/

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

char *get_path(char *command);
char **split_string(char *input, char **argv, int *argc);
char **replace_variables(char **argv, int argc);
int additional_functions(char **argv, int argc, int *command_pid);
pid_t exec(char **argv);
