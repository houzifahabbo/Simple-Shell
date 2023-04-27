#include "main.h"

void exec(char **argv){
    char *command = NULL ,*full_command_path = NULL;
    command = argv[0];
    full_command_path = get_path(command);
    if(execve(full_command_path, argv,NULL)== -1){
        perror("Shell Error");
    }
}

