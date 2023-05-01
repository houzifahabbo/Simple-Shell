/*
 *    File: additional_functions.c
 * Project: system-programming-project-1
 * Authors: Houzifa Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Contains additional functions for the command-line interface
 */

#include "main.h"

const char *additional_functions_str[] = {
  "help",
  "cd",
  "exit",
  "calc",
  "calendar",
  "fileinfo",
  "findreplace",
  "history",
  "count",
  "networkinfo"
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
    }  else  {
        int function_index = -1;
        for (int i = 0; i < additional_functions_str_size; i++) {
            if (strcmp(argv[1], additional_functions_str[i]) == 0) {
                function_index = i;
                break;
            }
        }
        if (function_index == 1) {
            printf("Eng: write cd path to change the diroctary\n");
            printf("Tur: cd path yazarak klasor değiştirin\n");
        } else if (function_index == 3) { /* hesapla */
            printf("Eng: Solve arithmetic operations for two number\n");
            printf("Tur: Verilen iki sayinin istenilen aritmetik islemini yapar\n");
                printf("calistirmak icin calc sonra yapmak istediginz islemi yaziniz\n");
                printf("ornek: calc 1 + 3\n");
        } else if (function_index == 4) { /* calendar */
            printf("Eng: Gives the calendar for the month in the given year\n");
            printf("Tur: Verilen yildaki ayin takvimini verir\n");
                printf("bir ay'in takvimi almak icin calender sonra istediginiz ay yaziniz ay'dan sonra istediginiz yil giriniz\n");
                printf("ornek : calender 4 2023 \n ");
        } else if (function_index == 5) { /* file_info */
            printf("Eng: Gives the information of the given file\n");
            printf("Tur: Verilen dosyanin bilgilerini verir\n");
                printf("calistirmak icin fileinfo sonra bligilarini ogrenmek istediginiz dosyanin bilgilerini yaziniz\n");
                printf("ornek : fileinfo test.txt\n");
        } else if (function_index == 6) { /* findreplace */
            printf("Eng: Replace the first given word with the second word\n ");
            printf("Tur: Verilen ilk kelimeyi ikinci kelime ile degistirir\n\n");
            printf("Eng: To run findreplace code you should write finderplace first_word second_word test.txt\n");
            printf("Tur: findreplace kodu calistirmak icin findreplace ilk_kelime ikinci_kelime test.txt yazmaniz gerekiyor\n");
        } else if (function_index == 7) { /* history */
            printf("Eng: Write history to display all history\nWrite history number so you can add to it or rerun it imeaditly\nNote: when you write history number you can't edit the command");
            printf("Tur: Tüm geçmişi görüntülemek için 'history' yazın\nGeçmişe bir numara eklemek veya hemen çalıştırmak için numarasını yazın\nNot: Geçmiş numarasını yazdığınızda komutu düzenleyemezsiniz\n");
        } else if (function_index == 8) { /* count */
            printf("Eng: Counting word that occurs in the file\n");
            printf("Tur: Verilen kelimeyi dosyada kac kez ciktigini sayar\n");
            printf("Tur: Calistirmak icin sunu yazin: count <filename> <word>\n");
        } else if (function_index == 9) { /* network */
            printf("Eng: Shows information about networkinfo\n");
            printf("Tur: Calistirmak icin networkinfo yazin\n");
        }
    }
}

/* Function to handle additional functions */
int additional_functions(char **argv, int argc, int *command_pid) {
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
                *command_pid = find_replace(argv, argc);
                return 1;
            } else if (i == 7) {
                return 2;
            } else if (i == 8) {
                *command_pid = count_word(argc,argv);
                return 1;
            } else if (i == 9) {
                network_info(argc,argv);
                return 1;
            }
        }
    }

    return 0;
}
