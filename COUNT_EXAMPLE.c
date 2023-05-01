#include "main.h"

#define MAX_WORD_LEN 100

int count_word(const char* filename, const char* word) {
    int count = 0;
    char buffer[MAX_WORD_LEN];
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fscanf(fp, "%s", buffer) != EOF) {
        if (strcmp(buffer, word) == 0) {
            count++;
        }
    }

    fclose(fp);
    return count;
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <filename> <word>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    const char* word = argv[2];

    int count = count_word(filename, word);

    if (count == -1) {
        return 1;
    }

    printf("The word '%s' occurs %d times in '%s'\n", word, count, filename);

    return 0;
}
