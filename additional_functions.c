#include "main.h"

const char *additional_functions_str[] = {
  "help",
  "cd",
  "exit"
};
const int additional_functions_str_size = (sizeof(additional_functions_str) / sizeof(additional_functions_str[0]));

void cd(char **argv){
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
         return 2;
      default:
        break;
    }
    return 0;
}