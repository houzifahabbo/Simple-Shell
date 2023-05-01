/*
 * File: get_path.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Retrieves the full path of a command executable
 */

#include "main.h"

/* Function: get_path
 * ------------------
 * Retrieves the full path of a command executable by searching the directories
 * specified in the PATH environment variable.
 *
 * command: the command for which to find the path
 *
 * returns: a dynamically allocated string containing the full path of the command
 *          executable if found, or NULL if not found or encountered an error
 */
char *get_path(char *command)
{
    char *path = getenv("PATH"); /* Get the value of the PATH environment variable */

    if (path == NULL) /* If PATH is not set */
        return NULL;

    const char *delimiter = ":";
    const size_t command_length = strlen(command); 

    char *path_copy = strdup(path); 
    if (path_copy == NULL) /* If memory allocation failed */
        return NULL;

    char *path_token = strtok(path_copy, delimiter); /* Get the first directory token from PATH */

    while (path_token != NULL) {
        const size_t directory_length = strlen(path_token);
        const size_t file_path_length = directory_length + command_length + 2;
        char *file_path = malloc(file_path_length); /* Allocate memory for the file path (+1 for null terminator) */

        if (file_path == NULL) { /* If memory allocation failed */
            free(path_copy);
            return NULL;
        }

        snprintf(file_path, file_path_length, "%s/%s", path_token, command); /* Construct the full file path */

        if (access(file_path, F_OK) == 0) { /* Check if the file path exists */
            free(path_copy);
            return file_path;
        }

        free(file_path);
        path_token = strtok(NULL, delimiter); /* Get the next directory token from PATH */
    }

    free(path_copy); 

    if (access(command, F_OK) == 0) /* Check if the command exists in the current directory */
        return strdup(command); 

    return NULL; /* Return NULL if the command was not found*/
}