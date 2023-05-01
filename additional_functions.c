/*
 *    File: additional_functions.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helany, Nour Chami, Muslim Umalatov
 * Purpose: 
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

pid_t findreplace(char **argv, int argc)
{
  
    if (argc != 4) {
        printf("Usage: findreplace <search_string> <replacement_string> <input_file>\n");
        return 0;
    }else{
        pid_t child_pid = fork();
        if (child_pid == 0) {


        char *search_string = argv[1];
        char *replacement_string = argv[2];
        char *input_file_name = argv[3];
        FILE *input_file = fopen(input_file_name, "r");

        if (!input_file) {
            printf("Error: cannot open input file '%s'\n", input_file_name);
            return 0;
        }

        char temp_file_name[] = "temp.txt"; /* geçici olarak açılacak kelimeler değişince silinecek */
        FILE *temp_file = fopen(temp_file_name, "w");

        if (!temp_file) {
            printf("Error: cannot open temporary file '%s'\n", temp_file_name);
            return 0;
        }

        char line[1000];

        while (fgets(line, 1000, input_file)) {
            char *pos = strstr(line, search_string);

            while (pos) {
                int offset = pos - line;
                memmove(pos + strlen(replacement_string), pos + strlen(search_string), strlen(pos) - strlen(search_string) + 1);
                memcpy(pos, replacement_string, strlen(replacement_string));
                pos = strstr(line + offset + strlen(replacement_string), search_string);
            }
            fputs(line, temp_file);
        }
        fclose(input_file);
        fclose(temp_file);

        if (remove(input_file_name) != 0) {
            printf("Error: cannot delete input file '%s'\n", input_file_name);
            return 0;
        }
        if (rename(temp_file_name, input_file_name) != 0) {
            printf("Error: cannot rename temporary file '%s' to '%s'\n", 
                temp_file_name, input_file_name);
            return 0;
        }
        exit(0);
        } else if (child_pid > 0) {
            waitpid(child_pid, NULL, 0);
            return child_pid;
        } else {
            perror("fork");
            exit(1);
        }
    }
}


/* file info */  
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


void hesapla(char **argv)
{
    double num1 = atoi(argv[1]), num2 =atoi(argv[3]);
    char op = argv[2][0];

    /*printf("Enter an arithmetic expression: "); */
  
 
    switch (op) {
        case '+':
            printf("= %.0f\n", num1 + num2);
            break;
        case '-':
            printf("= %.0f\n", num1 - num2);
            break;
        case '*':
            printf("= %.0f\n", num1 * num2);
            break;
        case '/':
            if (num2 == 0)
                printf("Error: Division by zero\n");
            else
                printf("= %.0f\n",num1 / num2);
            break;
        default:
            printf("Error: Invalid operator\n");
            break;
    }
}

void cd(char **argv)
{
    if (argv[1] == NULL)
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    else if (chdir(argv[1]) != 0)
        perror("lsh");
}

void help(char **argv)
{
    if(argv[1] == NULL){
        printf("Type program names and arguments, and hit enter\n");
            printf("The following are built in:\n");

            for (int i = 0; i < additional_functions_str_size; i++)
                printf("  %s\n", additional_functions_str[i]);

            printf("Use the man command for information on other programs.\n");
    }else{
        int function_index = -1;
        for (int i = 0; i < additional_functions_str_size; i++)
            if (strcmp(argv[1], additional_functions_str[i]) == 0) {
                function_index = i;
                break;
            }
        switch (function_index) {
            case 0:    /* help */
                break;
            case 1:    /* cd */
                break;
            case 2:
                break;
            case 3:    /* hesapla */      
                printf("Solve arithmetic operations for two number\n");
                printf("Verilen iki sayinin istenilen aritmetik islemini yapar\n");
                break;
            case 4:     /* calender */          
                printf("Gives the calendar for the month in the given year\n");
                printf("Verilen yildaki ayin takvimini verir\n");
                break;
            case 5:   /* file_info */    
                printf("Gives the infomation of the given file\n");
                printf("Veilen dosyanin bilgilerini verir\n");
                break;    
            case 6:   /* findreplace */  
                printf("Replace the first given word with the second word\n ");
                printf("Verilen ilk kelimeyi ikinci kelime ile degistirir\n");
                break;   
            case 7:
                printf("sgsdg\n");
                printf("dsgsdg\n");
                break;  
            default:
                break;
        }
    }
}

int additional_functions(char **argv, int argc, int *command_pid) {
    int function_index = -1;
    for (int i = 0; i < additional_functions_str_size; i++)
        if (strcmp(argv[0], additional_functions_str[i]) == 0) {
            function_index = i;
            break;
        }
    *command_pid = getpid();
    switch (function_index) {
        case 0:
            help(argv);
            return 1;
        case 1:
            cd(argv);
            return 1;
        case 2:
            return 3;
         case 3:
            hesapla(argv);
            return 1;
        case 4:
            calender(argv);
            return 1;
        case 5:
            file_info(argv);
            return 1;    
        case 6:  
            findreplace(argv,argc);
            return 1;
        case 7:
            return 2;  
        default:
            break;
    }
    return 0;
}