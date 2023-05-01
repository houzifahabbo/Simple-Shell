/*findreplace amaci bir metinde kelimeler degistirmek istiyorsak 
ilk once metindeki kelimeyi buluyor ondan sonra degistiriyor.*/

#include "main.h"

pid_t findreplace(char **argv, int argc) 
{
  
    if (argc != 4) {
        printf("Error: Yanlis bisey yazdin, kodu dogru calismasi icin help findreplace ten ogrenebilirsiniz. \n");
        return 0;
        /*Girdi parametre sayisinin dogru olup olmadigini kontrol ediyor. 
        Eger dogru degilse, kullaniciya dogru kullanimi gosterilerek fonksiyondan cikiyor*/
    }else{
        pid_t child_pid = fork(); 
        if (child_pid == 0) {
        /*Eger girdi parametre sayisi dogruy ise, bir child surec olusturuluyor*/

        char *search_string = argv[1];
        char *replacement_string = argv[2];
        char *input_file_name = argv[3];
        FILE *input_file = fopen(input_file_name, "r");

        if (!input_file) {
            printf("Error: cannot open input file '%s'\n", input_file_name);
            return 0;
        }

        char temp_file_name[] = "temp.txt"; 
        FILE *temp_file = fopen(temp_file_name, "w");
        /* gecici olarak acilacak kelimeler degisince silinecek */
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
