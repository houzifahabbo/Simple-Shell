/*************************************************************************************
 *
 *    File: get_path.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helany, Nour Chami, Muslim Umalatov
 * Purpose: 
 *
 *************************************************************************************/

#include "main.h"

char *get_path(char *command){
    char *path = getenv("PATH");
    if (path == NULL) return NULL;

    const char *delimiter = ":";
    const size_t command_length = strlen(command);

    char *path_copy = strdup(path);
    if (path_copy == NULL) return NULL;

    char *path_token = strtok(path_copy, delimiter);
    while (path_token != NULL) {
        const size_t directory_length = strlen(path_token);
        const size_t file_path_length = directory_length + command_length + 2;
        char *file_path = malloc(file_path_length); // +1 for null terminator

        if (file_path == NULL){
            free(path_copy);
            return NULL;
        }
        snprintf(file_path, file_path_length, "%s/%s", path_token, command);
        if (access(file_path, F_OK) == 0){
            free(path_copy);
            return file_path;
        }
        free(file_path);
        path_token = strtok(NULL, delimiter);
    }

    free(path_copy);

    if (access(command, F_OK) == 0) return strdup(command);

    return NULL;
}
