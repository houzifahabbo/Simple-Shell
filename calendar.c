/*
 *    File: calendar.c
 * Project: system-programming-project-1
 * Authors: Houzifa Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Contains a function to print a calendar for a given month and year
 */

#include "main.h"

int isLeapYear(int year) 
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void calendar(char **argv) 
{

    /* ay veya yil girmediyse uyari mesaji vermek icin */ 
     if (argv[1] == NULL || argv[2] == NULL) {
    printf("Please add month and year.\n");
    return;
   }
   else{
    int month = atoi(argv[1]);
    int year = atoi(argv[2]);
    int month_d = 0;     /* month_d: days of the month */
    int week_d = 0;      /* week_d: days of the week  */
    char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August",
                      "September", "October", "November", "December"};
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int isLeap = isLeapYear(year);
    month_d = daysInMonth[month - 1];

    if (month == 2 && isLeap)
        month_d = 29;

    /* takvimin basligi yazdirmak */
    printf("\n%s %d\n", months[month - 1], year);
    printf("Sun Mon Tue Wed Thu Fri Sat\n");

    /* haftanin ilk gununu ve ayin hangi gunde basladigini bulmak */
    int startingDay = 1; /* 1st January 0001 was Monday */

    for (int i = 1; i < year; i++) 
        startingDay += isLeapYear(i) ? 366 : 365;
    

    for (int i = 1; i < month; i++) {
        startingDay += daysInMonth[i - 1];

        if (i == 2 && isLeapYear(year)) 
            startingDay++; /* Add an extra day for leap year February */
    }

    startingDay %= 7;

    for (int i = 0; i < startingDay; i++) 
        printf("    ");
    

    for (int i = 1; i <= month_d; i++) {
        printf("%3d ", i);
        if ((i + startingDay) % 7 == 0 || i == month_d) 
            printf("\n");
        }
     }
} 

