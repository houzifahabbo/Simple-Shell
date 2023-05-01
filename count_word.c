#include "main.h"
#define MAX_WORD_LEN 100

void count_word(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <word>\n", argv[0]);
        exit(1);
    }

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
}