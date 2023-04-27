#include "main.h"
char** split_string(char* input ,char ** argv) {
    int num_tokens,i;
    const char* delim = " \n";
    char* input_copy= strdup(input);
    char* token = strtok(input, delim);;
    
    while (token != NULL) {
        num_tokens ++;
        token = strtok(NULL, delim);
    }

    argv = malloc((num_tokens + 1) * sizeof(char*));

    // Tokenize the input string and store each token in argv
    token = strtok(input_copy, delim);

    while (token != NULL) {
        argv[i] = strdup(token); // Make a copy of each token
        i++;
        token = strtok(NULL, delim);
    }
    argv[i] = NULL; // Set the last element of argv to NULL as a sentinel value

    free(input_copy); // Free the memory allocated for the input copy
    return argv;
}