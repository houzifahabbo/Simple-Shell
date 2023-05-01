/*
 * File: count_word.c
 * Project: system-programming-project-1
 * Authors: Houzifa Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Contains a function to count the occurrences of a word in a file
 */
#include "main.h"
#define MAX_WORD_LEN 100

pid_t count_word(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <word>\n", argv[0]);
        exit(1);
    }
    pid_t child_pid = fork(); 
    if (child_pid == 0) {
    
        /* First and second word in terminal as filename and word */
        const char* filename = argv[1];
        const char* word = argv[2];

        int count = 0;
        char buffer[MAX_WORD_LEN];

        /* Opening file just for reading */
        FILE* fp = fopen(filename, "r");

        /* Check for error */
        if (fp == NULL) {
            perror("Error opening file");
            exit(1);
        }

        /* Counting words */
        while (fscanf(fp, "%s", buffer) != EOF) {
            if (strcmp(buffer, word) == 0) {
                count++;
            }
        }

        /* Printing message to shell */

        printf("The word '%s' occurs %d times in '%s'\n", argv[2], count, argv[1]);

        fclose(fp);
        exit(0);
    } else if (child_pid > 0) {
        waitpid(child_pid, NULL, 0);
        return child_pid;
    } else {
        perror("fork");
        exit(1);
    }
}