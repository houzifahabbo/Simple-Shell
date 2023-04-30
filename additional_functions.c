#include "main.h"

const char *additional_functions_str[] = {
  "help",
  "cd",
  "exit",
  "calc",
  "calender",
  "fileinfo"
};
const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));

 
int takvim (){
int month, year, month_d, week_d=0;  /*mounth_d: days of the month, week_d: days of the week*/ 
   char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    /* ayin gunleri hesaplamak */ 
   scanf("%d %d", &month, &year);
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
int file_info (){
  char file_name[10];
  struct stat file_i; /*dosyanin biligilerini depolamak icin*/
  scanf("%s", file_name);
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
     for(int j = i + 1; j < 10; j++) {
       printf("%c", file_name[j]);
     }
     printf("\n");
     break;
    }
  }
}


int hesapla(){
 double num1, num2;
    char op;

    //printf("Enter an arithmetic expression: ");
    scanf("%lf %c %lf", &num1, &op, &num2);

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
  } else {
    if (chdir(argv[1]) != 0) {
      perror("lsh");
    }
  }
}
int help(char **args)
{
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  for (int i = 0; i < additional_functions_str_size; i++) {
    printf("  %s\n", additional_functions_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int additional_functions(char** argv)
{
    int function_index = 0;
    for (int i = 0; i < additional_functions_str_size; i++) {
        if (strcmp(argv[0], additional_functions_str[i]) == 0) {
            function_index = i + 1;
            break;
        }
    }
    switch (function_index) {
        case 1:
            help(argv);
            return 1;
        case 2:
            cd(argv);
            return 1;
        case 3:
            exit(0);    
        case 4:
            hesapla();
            return 1;
        case 5:
            takvim();
            return 1;
        case 6:
            file_info();
            return 1;    
        default:
            break;
    }
    return 0;
}