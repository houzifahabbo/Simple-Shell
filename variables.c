/*
 *    File: variables.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: 
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

char *(*variable) (const char *) = &getenv;
const int variables_str_size = sizeof(variables_str) / sizeof(variables_str[0]);

char **replace_variables(char **argv, int argc)
{
    for (int i = 0; i < argc; i++) {
        if (argv[i] != NULL) {
            for (int j = 0; j < variables_str_size; j++) {
                char *pos = strstr(argv[i], variables_str[j]);
                if (pos != NULL) {
                    size_t searchLen = strlen(variables_str[j]);
                    char *value = getenv(variables[j]);
                    if (value != NULL) {
                        size_t replaceLen = strlen(value);
                        size_t newPosLen = strlen(pos + searchLen);
                        char *temp = malloc(replaceLen + newPosLen + 1);
                        if (temp != NULL) {
                            memcpy(temp, value, replaceLen);
                            memcpy(temp + replaceLen, pos + searchLen, newPosLen + 1);
                            memcpy(pos, temp, replaceLen + newPosLen + 1);
                        }
                    }
                }
            }
        }
    }
    return argv;
}
