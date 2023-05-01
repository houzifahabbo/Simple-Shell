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
  "findreplace"
};

const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));

int findreplace(char **argv, int argc)
{
    if (argc != 4) {
        printf("Usage: findreplace <search_string> <replacement_string> <input_file>\n");
        exit(1);
    }

    char *search_string = argv[1];
    char *replacement_string = argv[2];
    char *input_file_name = argv[3];
    FILE *input_file = fopen(input_file_name, "r");

    if (!input_file) {
        printf("Error: cannot open input file '%s'\n", input_file_name);
        exit(1);
    }

    char temp_file_name[] = "temp.txt"; /* geçici olarak açılacak kelimeler değişince silinecek */
    FILE *temp_file = fopen(temp_file_name, "w");

    if (!temp_file) {
        printf("Error: cannot open temporary file '%s'\n", temp_file_name);
        exit(1);
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
        exit(1);
    }
    if (rename(temp_file_name, input_file_name) != 0) {
        printf("Error: cannot rename temporary file '%s' to '%s'\n", 
              temp_file_name, input_file_name);
        exit(1);
    }
}

 

int takvim (char **argv)
{
    int month = atoi(argv[1]);
    int year = atoi(argv[2]);
    int month_d;     /* month_d: days of the month */
    int week_d = 0;  /* week_d: days of the week */ 
    char *months[] = {"January", "February", "March",
                      "April", "May", "June", "July", "August",
                      "September", "October", "November", "December"};

    /* ayin gunleri hesaplamak */ 
    /* scanf("%d %d", &month, &year); */
    switch (month) {
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                month_d = 29;
            else
                month_d = 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            month_d = 30;
            break;
        default:
            month_d = 31;
            break;
            
int takvim (char** argv){
int month = atoi(argv[1]), year=atoi(argv[2]), month_d, week_d=0;  /*mounth_d: days of the month, week_d: days of the week*/ 
   char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    /* ayin gunleri hesaplamak */ 
  // scanf("%d %d", &month, &year);
   switch (month) {
     case 2:
       if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
         month_d = 29;
       } 
       else {
         month_d = 28;
       }
     break;
     case 4:
     case 6:
     case 9:
     case 11:
       month_d = 30;
     break;
     default:
       month_d = 31;
     break;
   } 
    /*takvimin basligi yazdimak*/
    printf("\n%s %d\n", months[month - 1], year);
    printf("Sun Mon Tue Wed Thu Fri Sat\n");
    /*haftanin ilk gununu va ayin hangi gunde basladigini bulmak*/
    for (int i = 1; i < year; i++) {
      if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
        week_d = week_d + 366;
      }
      else {
        week_d = week_d + 365;
      }
    }
    for (int i = 1; i < month; i++) {
      switch (i) {
       case 2:
          if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            week_d = week_d + 29;
          }
          else {
            week_d = week_d + 28;
          }
       break;
       case 4:
       case 6:
       case 9:
       case 11:
          week_d = week_d + 30;
       break;
       default:
          week_d = week_d + 31;
       break;
     }
    }
    week_d = (week_d + 1) % 7;
    for (int i = 0; i < week_d; i++) {
      printf("    ");
    }
    for (int i = 1; i <= month_d; i++) {
      printf("%3d ", i);
      if ((i + week_d) % 7 == 0 || i == month_d) {
        printf("\n");
      }
    }
}


/*file info*/  
int file_info (char** argv){
  char *file_name = argv [1];
  struct stat file_i; /*dosyanin biligilerini depolamak icin*/
  
  if (stat(file_name, &file_i) < 0) {
    printf("Error: Unable to stat file.\n");
    return 1;
  }
  printf("File size: %ld bytes\n", file_i.st_size);
  printf("Time created: %s", ctime(&file_i.st_ctime));
  /*dosyanin tipini bulup yazdirmak */
  for( int i = 0; i < 10; i++) {
    if(file_name[i] == '.') {
     printf("Type of the file: ");
     for(int j = i + 1; j < 9; j++) {
       printf("%c", file_name[j]);
     }
     printf("\n");
     break;
    }
  }
}


void hesapla(char** argv){
 double num1 = atoi(argv[1]), num2 =atoi(argv[3]);
    char op= argv[2][0];

    //printf("Enter an arithmetic expression: ");
  
 
    switch(op) {
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
            if (num2 == 0) {
                printf("Error: Division by zero\n");
            }
            else {
                printf("= %.0f\n",num1 / num2);
            }
            break;
        default:
            printf("Error: Invalid operator\n");
            break;
    }
}
void cd(char **argv){
    if (argv[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else if (chdir(argv[1]) != 0){
        perror("lsh");
    }

    /* takvimin basligi yazdirmak */
    printf("\n%s %d\n", months[month - 1], year);
    printf("Sun Mon Tue Wed Thu Fri Sat\n");

    /* haftanin ilk gununu va ayin hangi gunde basladigini bulmak */

    for (int i = 1; i < year; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0))
            week_d = week_d + 366;
        else
            week_d = week_d + 365;
    }
    for (int i = 1; i < month; i++) {
        switch (i) {
            case 2:
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                    week_d = week_d + 29;
                else
                    week_d = week_d + 28;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                week_d = week_d + 30;
                break;
            default:
                week_d = week_d + 31;
                break;
        }
    }
    week_d = (week_d + 1) % 7;

    for (int i = 0; i < week_d; i++) {
      printf("    ");
    }

    for (int i = 1; i <= month_d; i++) {
        printf("%3d ", i);
        if ((i + week_d) % 7 == 0 || i == month_d)
            printf("\n");
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

void help(char** argv){
  int function_index = 0;
    for (int i = 0; i < additional_functions_str_size; i++){
        if (strcmp(argv[1], additional_functions_str[i]) == 0){
            function_index = i + 1;
            break;
        }
    }
  switch (function_index){
    
        case 1:    /*  help  */
            break;
        case 2:    /*  cd  */
            break;
        case 3:
            break;
         case 4:/*  hesapla  */      
            printf("Solve arithmetic operations for two number.\n");
            printf("Verilen iki sayinin istenilen aritmatik islemi yapar\n");

            break;
        case 5: /*  takvim  */          
           printf("Gives the calendar for the month in the given year. \n");
           printf("Verilen yildaki ayin takvimini verir. \n");
            break;
        case 6: /* file_info  */    
            printf("Gives the infomation of the given file.\n");
            printf("Veilen dosyanin bilgilerini verir\n");
            break;    
        case 7: /* findreplace */  
            printf("Replace the first given word with the second word.\n ");
            printf("Verilen ilk kelimeyi ikinci kelime ile degistirir. \n");
            break;   
        default:
         printf("Type program names and arguments, and hit enter.\n");
        printf("The following are built in:\n");
        for (int i = 0; i < additional_functions_str_size; i++){
        printf("  %s\n", additional_functions_str[i]);
       }
       printf("Use the man command for information on other programs.\n");


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
    int function_index = 0;
    for (int i = 0; i < additional_functions_str_size; i++)
        if (strcmp(argv[1], additional_functions_str[i]) == 0) {
            function_index = i + 1;
            break;
        }
    switch (function_index) {
        case 1:    /* help */
            break;
        case 2:    /* cd */
            break;
        case 3:
            break;
        case 4:    /* hesapla */      
            printf("Solve arithmetic operations for two number\n");
            printf("Verilen iki sayinin istenilen aritmetik islemini yapar\n");
            break;
        case 5:     /* takvim */          
            printf("Gives the calendar for the month in the given year\n");
            printf("Verilen yildaki ayin takvimini verir\n");
            break;
        case 6:   /* file_info */    
            printf("Gives the infomation of the given file\n");
            printf("Veilen dosyanin bilgilerini verir\n");
            break;    
        case 7:   /* findreplace */  
            printf("Replace the first given word with the second word\n ");
            printf("Verilen ilk kelimeyi ikinci kelime ile degistirir\n");
            break;   
        default:
            printf("Type program names and arguments, and hit enter\n");
            printf("The following are built in:\n");

            for (int i = 0; i < additional_functions_str_size; i++)
                printf("  %s\n", additional_functions_str[i]);

            printf("Use the man command for information on other programs.\n");
            break;
    }
}

int additional_functions(char **argv, int argc, int *command_pid)
{
    int function_index = 0;
    for (int i = 0; i < additional_functions_str_size; i++)
        if (strcmp(argv[0], additional_functions_str[i]) == 0) {
            function_index = i + 1;
            break;
        }
    *command_pid = getpid();
    switch (function_index) {
        case 1:
            help(argv);
            return 1;
        case 2:
            cd(argv);
            return 1;
        case 3:
          
            return 2;
         case 4:
            hesapla(argv);
            return 1;
        case 5:
            takvim(argv);
            return 1;
        case 6:
            file_info(argv);
            return 1;    
        case 7:  
            findreplace(argv,argc);
            return 1;   
        default:
            break;
    }
    return 0;
}