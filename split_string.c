/*
 *    File: split_string.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: 
 */

#include "main.h"

char **split_string(char *input, char **argv, int *argc)
{
    int i = 0;
    const char *delim = " \n";
    char *input_copy= strdup(input);
    char *token = strtok(input, delim);
    *argc = 0;

    while (token != NULL) {
        (*argc)++;
        token = strtok(NULL, delim);
    }
    
    argv = malloc((*argc + 1) * sizeof(char*));

    /* Tokenize the input string and store each token in argv */
    token = strtok(input_copy, delim);

    while (token != NULL) {
        argv[i] = strdup(token); /* Make a copy of each token */
        i++;
        token = strtok(NULL, delim);
    }

    argv[i] = NULL; /* Set the last element of argv to NULL as a sentinel value */

    free(input_copy);
    free(token);
    return argv;
}