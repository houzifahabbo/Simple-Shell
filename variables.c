/*
 *  File: variables.c
 *  Project: system-programming-project-1
 *  Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 *  Purpose: Defines variables and functions related to variable replacement
 */

#include "main.h"

char *variables[] = {
"HOME",
"PATH",
"USER",
"HOME",
};

char *variables_str[] = {
"$HOME",
"$PATH",
"$USER",
"~",
};

/* Function pointer to getenv() */
char *(*variable)(const char *) = &getenv;

const int variables_str_size = sizeof(variables_str) / sizeof(variables_str[0]);

/*

    Function: replace_variables
    Replaces variables in the command arguments with their corresponding values.
    argv: An array of command arguments
    argc: The number of command arguments
    returns: The modified argv array with variables replaced
    */
char **replace_variables(char **argv, int argc)
{
    for (int i = 0; i < argc; i++) {
        if (argv[i] != NULL) {
            for (int j = 0; j < variables_str_size; j++) {
                char *pos = strstr(argv[i], variables_str[j]);  /* Find the position of variables_str[j] within argv[i] */
                if (pos != NULL) {
                    size_t searchLen = strlen(variables_str[j]);  
                    char *value = getenv(variables[j]);  /* Get the value of variables[j] from the environment */
                    if (value != NULL) {
                        size_t replaceLen = strlen(value); 
                        size_t newPosLen = strlen(pos + searchLen);  /* Calculate the length of the remaining characters after the variable position */
                        char *temp = malloc(replaceLen + newPosLen + 1);
                        if (temp != NULL) {
                            memcpy(temp, value, replaceLen);  /* Copy the replacement value into the temporary string */
                            memcpy(temp + replaceLen, pos + searchLen, newPosLen + 1);  /* Copy the remaining characters after the variable position into the temporary string */
                            memcpy(pos, temp, replaceLen + newPosLen + 1);  /* Replace the variable with its value in argv[i] */
                        }
                    }
                }
            }
        }
    }
    return argv;  /* Return the modified argv array */
}
