/*
 *    File: exec.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helany, Nour Chami, Muslim Umalatov
 * Purpose: Implements the exec function to execute a command in a child process.
 */

#include "main.h"

/*
 * Executes the specified command by forking a child process and invoking execve.
 * Parameters:
 *   - argv: A null-terminated array of strings representing the command and its arguments.
 * Returns:
 *   - The process ID of the child process.
 */
pid_t exec(char **argv)
{
    char *command = NULL, *full_command_path = NULL, *envp[] = { "TERM=xterm", NULL };
    pid_t child_pid = fork();

    if (child_pid == 0) {
        command = argv[0];
        full_command_path = get_path(command);

        if (execve(full_command_path, argv, envp) == -1)
            perror("Shell Error");

        exit(0);
    } else if (child_pid > 0) {
        waitpid(child_pid, NULL, 0);
        return child_pid;
    } else {
        perror("fork");
        exit(1);
    }
}
