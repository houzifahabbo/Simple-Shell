/*
 *    File: additional_functions.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
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
  "history",
  "count",
  "network"
};

const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));


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
    double num1 = atoi(argv[1]), num2 = atoi(argv[3]);
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
    } else {
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
                printf("Eng: Solve arithmetic operations for two number\n");
                printf("Tur: Verilen iki sayinin istenilen aritmetik islemini yapar\n");
                break;
            case 4:     /* calendar */          
                printf("Eng: Gives the calendar for the month in the given year\n");
                printf("Tur: Verilen yildaki ayin takvimini verir\n");
                break;
            case 5:   /* file_info */    
                printf("Eng: Gives the infomation of the given file\n");
                printf("Tur: Verilen dosyanin bilgilerini verir\n");
                break;    
            case 6:   /* findreplace */  
                printf("Eng: Replace the first given word with the second word\n ");
                printf("Tur: Verilen ilk kelimeyi ikinci kelime ile degistirir\n\n");
                printf("Eng: To run findreplace code you should write finderplace first_word second_word test.txt in terminal\n");
                printf("Tur: findreplace kodu calistirmak icin findreplace ilk_kelime ikinci_kelime test.txt terminalda yazman gerekiyor\n");

                break;   
            case 7:
                printf("sgsdg\n");
                printf("dsgsdg\n");
                break;
            case 8:   /* count */
                printf("Eng: Counting word that occurs in the file\n");
                printf("Tur: Verilen kelimeyi dosyada kac kez ciktigini sayar\n");
                printf("Tur: Calistirmak icin sunu yazin: count <filename> <word> ");
                break;
            case 9:   /* network */
                printf("Shows information about network\n");
                printf("To display information just write network\n");
                break;
            default:
                break;
        }
    }
}

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

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 2
#endif

int network_info(int argc, char *argv[]) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    printf("Interface\tAddress\n");

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            printf("%s:\t%s\n", ifa->ifa_name, host);
        }
    }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
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
        case 8:
            count_word(argc, argv);
            return 1;
        case 9:
            network_info(argc, argv);
            return 1;
        default:
            break;
    }
    return 0;
}
