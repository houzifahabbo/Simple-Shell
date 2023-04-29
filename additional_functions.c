#include "main.h"

const char *additional_functions_str[] = {
  "help",
  "cd",
  "exit",
  "findreplace",
};
const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));

int findreplace(char **argv,int argc) {
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
    char temp_file_name[] = "temp.txt"; // geçici olarak açılacak kelimeler değişince silinecek
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
        printf("Error: cannot rename temporary file '%s' to '%s'\n", temp_file_name, input_file_name);
        exit(1);
    }
}
void cd(char *argv){
  if (argv[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(argv[1]) != 0) {
      perror("lsh");
    }
  }
}
int help()
{
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  for (int i = 0; i < additional_functions_str_size; i++) {
    printf("  %s\n", additional_functions_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int additional_functions(char** argv, int argc, int * command_pid)
{
    int function_index = 0;
    for (int i = 0; i < additional_functions_str_size; i++) {
        if (strcmp(argv[0], additional_functions_str[i]) == 0) {
            function_index = i + 1;
            break;
        }
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
            exit(0);
        default:
            break;
    }
    return 0;
}