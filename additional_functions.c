#include "main.h"

const char *additional_functions_str[] = {
  "help",
  "cd",
  "exit",
  "findreplace",
  "history"
};
const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));

pid_t findreplace(char **argv,int argc) {
  
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
    char temp_file_name[] = "temp.txt"; // geçici olarak açılacak kelimeler değişince silinecek
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
        printf("Error: cannot rename temporary file '%s' to '%s'\n", temp_file_name, input_file_name);
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

void cd(char **argv) {
  if (argv[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  }
  if (chdir(argv[1]) != 0) {
    perror("lsh");
  }
}

void help(char **argv) {
  if (argv[1] == NULL) {
    printf("help [command name]\n");
  } else {
      int function_index = -1;
      for (int i = 0; i < additional_functions_str_size; i++) {
        if (strcmp(argv[1], additional_functions_str[i]) == 0) {
          function_index = i;
          break;
        }
      }
      switch (function_index) {
        case 0:
          printf(" \n");
          break;
        case 1:
          printf(" \n");
          break;
        case 3:
          printf("fggd\n");
          break;
        default:
          printf("fggd\n");
          break;
      }
  }
}

int additional_functions(char** argv, int argc, int * command_pid,char ** history_str,int history_str_size) {
    int function_index = -1;
    for (int i = 0; i < additional_functions_str_size; i++) {
        if (strcmp(argv[0], additional_functions_str[i]) == 0) {
            function_index = i;
            break;
        }
    }
    if (function_index >= 0) {
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
                     *command_pid = findreplace(argv, argc);
                     return 1;
                case 4:
                      return 2;
                default:
                    return 0;
            }
}
return 0;
} 