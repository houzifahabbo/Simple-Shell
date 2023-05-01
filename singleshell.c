/*
 * File: singleshell.c
 * Project: system-programming-project-1
 * Authors: Houzifa Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Implements a single-shell command-line interface
 * Note: I used readline library just for handling navigation, command history, and code completion
 */

#include "main.h"
#define INBUF_SIZE 256

/* these should be the same as multishell.c */
#define MY_FILE_SIZE 1024
#define MY_SHARED_FILE_NAME "/sharedlogfile"

char *addr = NULL;
int fd = -1;

int initmem()
{
    fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0);
    if (fd < 0) {
        perror("singleshell.c:fd:line31");
        exit(1);
    }
    if (ftruncate(fd, MY_FILE_SIZE) == -1) {
        perror("singleshell.c:ftruncate:");
        close(fd);
        exit(1);
    }
    addr = mmap(NULL, MY_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == NULL) {
        perror("singleshell.c:mmap:");
        close(fd);
        exit(1);
    }
    return 0;
}

/* Function to generate the command prompt */
char* prompt()
{
    char cwd[256] = {'\0'};  /* Buffer to store the current working directory */
    char prompt[1024] = {'\0'};  /* Buffer to store the prompt text */
    
    /* Get the current working directory */
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");  /* Print error message if getcwd fails */
        exit(1);
    }
    
    /* Format the prompt text with color codes and current working directory */
    snprintf(prompt, sizeof(prompt),
             "\033[0;36m%s\033[0;37m:\033[0;32m%s\033[0;37m$ \033[0m",
             getenv("USER"), cwd);
    
    return strdup(prompt);  /* Return a dynamically allocated copy of the prompt text */
}

/* Function to display command history */
char* history(char **argv, char **history_str, int history_str_size)
{
    if (argv[1] == NULL) {
        /* No argument provided, display entire command history */
        for (int index = 0; index < history_str_size; index++) {
            fprintf(stdout, "%d:  %s\n", index + 1, history_str[index]);
            fflush(stdout);
        }
        return NULL;
    } else {
        /* Argument provided, retrieve specific history entry */
        int index = atoi(argv[1]);
        index--;
        if (index >= 0 && index < history_str_size) {
            /* Valid history index */
            if (strcmp(history_str[index], "null") == 0)
                return NULL;  /* Entry is empty */
            else
                return history_str[index];  /* Return history entry */
        }
    }
    return NULL;  /* Invalid history index */
}

int main(int argc, char **argv)
{
    char *input;
    char *history_str[10] = {"null", "null", "null", "null", "null",
                             "null", "null", "null", "null", "null"};
    int flag_history = 0;   /* Flag to control the flow of execution when handling command history. */
    int index = 0;
    int history_index = 0;
                              
    int history_str_size = sizeof(history_str) / sizeof(history_str[0]);
    pid_t command_pid;
    
    initmem();
    log_message(NULL, &command_pid, 1, argc, index, addr);
    
    while (1)
    {
        command_pid = getpid();
        char *prompt_txt = prompt();

        /*When flag_history is set to 0 it will get input from the user by using readline
         *,and when it is set to any other value it will execute history 
         */
        if (flag_history == 0) {
            input = readline(prompt_txt);
        } else { 
            flag_history = 0;
            char *history_input = history(argv, history_str, history_str_size);
            if (history_input != NULL) {
                strcat(prompt_txt, history_input);
                snprintf(input, sizeof(input), "%s %s", history_input, readline(prompt_txt));
            } else {
                continue;
            }
        }
        
        /*add input to readline's and custom history function's history_str */
        if (input) {
            add_history(input);
            if (strncmp("history", input, 7) != 0) {
                history_str[history_index] = strdup(input);
            }
        }
        /* The argv array is updated by the finction which tokenizes the input string based on whitespace*/
        argv = split_string(input, argv, &argc);
        if (argv[0] == NULL) {
            continue;
        }
        
        /* Replace varibles in the input by there value e.g: ~ = /home/USER */
        argv = replace_variables(argv, argc);
        
        /* Additional_functions will search for the command to run it if there is not
         * x = 0: Execute the command
         * x = 2: Set flag to 1 and continue the loop to handle command history
         * x = 3: Log the command, free memory, and exit the program
         */
        int result = additional_functions(argv, argc, &command_pid);
        
        if (result == 0)
            command_pid = exec(argv);
        else if (result == 2) {
            flag_history = 1;
            continue;
        } else if (result == 3) {
            log_message(argv, &command_pid, 2, argc, index, addr);
            free(argv);
            break;
        }
        /* Log commands*/
        log_message(argv, &command_pid, 0, argc, index, addr);
        /* Set indexes*/
        if (history_index == history_str_size)
            history_index = 0;
        else
            history_index++;
        index++;
        /* Free variables*/
        free(argv);
        free(input);
    }
    
    /* Unmap the shared memory */
    munmap(addr, MY_FILE_SIZE);
    
    /* Close the shared memory file */
    close(fd);

    exit(0);
}

