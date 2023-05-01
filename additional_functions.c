/*
 *    File: additional_functions.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helany, Nour Chami, Muslim Umalatov
 * Purpose: Contains additional functions for the command-line interface
 */

#include "main.h"

const char *additional_functions_str[] = {
  "help",
  "cd",
  "exit",
  "calc",
  "calender",
  "fileinfo",
  "findreplace",
  "history"
};

const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));

/* Function to display file information */
int file_info(char **argv)
{
    char *file_name = argv[1];
    struct stat file_i; /* dosyanin bilgilerini depolamak icin */
  
    if (stat(file_name, &file_i) < 0) {
        printf("Error: Unable to stat file.\n");
        return 1;
    }
    printf("File size: %ld bytes\n", file_i.st_size);
    printf("Time created: %s", ctime(&file_i.st_ctime));
    /* dosyanin tipini bulup yazdirmak */
    for (int i = 0; i < 10; i++)
        if (file_name[i] == '.') {
            printf("Type of the file: ");

            for(int j = i + 1; j < 9; j++)
                printf("%c", file_name[j]);
                
            printf("\n");
            break;
        }
}

/* Function to perform arithmetic calculations */
void calculate(char **argv)
{
    double num1 = strtod(argv[1], NULL);
    double num2 = strtod(argv[3], NULL);
    char op = argv[2][0];

    if (op == '+') {
        printf("= %.2f\n", num1 + num2);
    } else if (op == '-') {
        printf("= %.2f\n", num1 - num2);
    } else if (op == '*') {
        printf("= %.2f\n", num1 * num2);
    } else if (op == '/') {
        if (num2 == 0) {
            printf("Error: Division by zero\n");
        } else {
            printf("= %.2f\n", num1 / num2);
        }
    } else {
        printf("Error: Invalid operator\n");
    }
}

/* Function to change directory */
void change_directory(char **argv)
{
    if (argv[1] == NULL)
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    else if (chdir(argv[1]) != 0)
        perror("lsh");
}

/* Function to display help information */
void display_help(char **argv) {
    if (argv[1] == NULL) {
        printf("Type program names and arguments, and hit enter\n");
        printf("The following are built in:\n");

        for (int i = 0; i < additional_functions_str_size; i++)
            printf("  %s\n", additional_functions_str[i]);

        printf("Use the man command for information on other programs.\n");
    } else {
        int function_index = -1;
        for (int i = 0; i < additional_functions_str_size; i++) {
            if (strcmp(argv[1], additional_functions_str[i]) == 0) {
                function_index = i;
                break;
            }
        }
        if (function_index == 0) {

        } else if (function_index == 1) {

        } else if (function_index == 3) {
            printf("Solve arithmetic operations for two number\n");
            printf("Verilen iki sayinin istenilen aritmetik islemini yapar\n");
        } else if (function_index == 4) {
            printf("Gives the calendar for the month in the given year\n");
            printf("Verilen yildaki ayin takvimini verir\n");
        } else if (function_index == 5) {
            printf("Gives the infomation of the given file\n");
            printf("Veilen dosyanin bilgilerini verir\n");
        } else if (function_index == 6) {
            printf("Replace the first given word with the second word\n ");
            printf("Verilen ilk kelimeyi ikinci kelime ile degistirir\n");
        } else if (function_index == 7) {
            printf("sgsdg\n");
            printf("dsgsdg\n");
        }
    }
}


/* Function to handle additional functions */
int additional_functions(char **argv, int argc, int *command_pid) {
    *command_pid = getpid();

    for (int i = 0; i < additional_functions_str_size; i++) {
        if (strcmp(argv[0], additional_functions_str[i]) == 0) {
            if (i == 0) {
                display_help(argv);
                return 1;
            } else if (i == 1) {
                change_directory(argv);
                return 1;
            } else if (i == 2) {
                return 3;
            } else if (i == 3) {
                calculate(argv);
                return 1;
            } else if (i == 4) {
                calendar(argv);
                return 1;
            } else if (i == 5) {
                file_info(argv);
                return 1;
            } else if (i == 6) {
                find_replace(argv, argc);
                return 1;
            } else if (i == 7) {
                return 2;
            }
        }
    }

    return 0;
}
