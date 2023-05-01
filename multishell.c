/*
 * File: multishell.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Implements a multi-shell command-line interface
 */

#include "main.h"

#define MY_FILE_SIZE 1024
#define MY_SHARED_FILE_NAME "/sharedlogfile"

#define MAX_SHELL 10
#define DEFAULT_NSHELL 2
char *addr = NULL; 
int fd = -1; 

/* Function: initmem
 * ------------------
 * Initializes shared memory for inter-process communication.
 *
 * returns: 0 if successful, otherwise terminates the program
 */
int initmem()
{
    fd = shm_open(MY_SHARED_FILE_NAME, O_CREAT | O_RDWR | O_TRUNC, 0666); 
    if (fd < 0) {
        perror("multishell.c: open file:");
        exit(1);
    }
    if (ftruncate(fd, 1024) == -1) { 
        perror("ftruncate");
        exit(1);
    }

    addr = mmap(NULL, MY_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    if (addr == NULL) {
        perror("mmap:");
        exit(1);
    }
    return 0;
}

/* Function: save_to_file
 * ----------------------
 * Saves the shared memory content to a file with a timestamp in the filename.
 */
void save_to_file()
{
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);
    char file_name[50];
    strftime(file_name, sizeof(file_name), "shelllog-[%d-%m-%Y %H:%M:%S].txt", time_info); /* Create a filename with a timestamp */

    FILE *log_file = fopen(file_name, "w"); /* Open the file for writing */
    if (log_file == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(log_file, "%s", addr); /* Write the shared memory content to the file */
    fclose(log_file);
}

int main(int argc, char **argv)
{
    int nshell = DEFAULT_NSHELL;

    if (argc > 1) {
        nshell = atoi(argv[1]); /* Get the number of shells from the command line arguments */
        if (nshell <= 0 || nshell > MAX_SHELL) {
            fprintf(stderr, "Invalid number of shells. Setting to default: %d\n", DEFAULT_NSHELL);
            nshell = DEFAULT_NSHELL;
        }
    }
    initmem(); 

    pid_t child_pids[MAX_SHELL];

    for (int i = 0; i < nshell; i++) {
        pid_t child_pid = fork(); /* Create a new child process for each shell*/ 
        if (child_pid == 0) { 
            char title[20];
            snprintf(title, sizeof(title), "singleshell %d", i + 1); /* Create a title for the shell window */

            /* Execute the singleshell program in a new xterm window */
            if (execlp("xterm", "xterm", "-fa", "Ubuntu Mono", "-fs", "15", "-T", title, "-e", "./singleshell","singleshell", NULL) == -1) {
                perror("execlp");
                exit(1);
            }
            exit(0);
        } else if (child_pid > 0) {
            child_pids[i] = child_pid; /* Store the child process ID */
        } else {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < nshell; i++) {
        waitpid(child_pids[i], NULL, 0); /* Wait for each child process to finish */
        save_to_file(); /* Save the shared memory content to a file */
    }

    munmap(addr, 1024); // Unlink the mapped memory

    close(fd); // Close the shared memory file

    return 0;
}

