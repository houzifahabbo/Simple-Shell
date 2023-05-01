/*
 * File: log_message.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: Implements the log_message function to write log messages to a shared memory address.
 */

#include "main.h"
#define INBUF_SIZE 256
#define MY_FILE_SIZE 1024

/* Function to log a message in shared memory */
void log_message(char **message, pid_t *command_pid, int flag, int argc, int index, char *addr)
{
    /* Buffer to hold the log message */
    char buffer[INBUF_SIZE] = {'\0'};
    size_t buffer_length = strlen(buffer);

    /* Calculate the remaining space in the shared memory for writing the log message */
    int bytes_written;
    int remaining_space = MY_FILE_SIZE - strlen(addr) - 1;
    /* Get the process ID and parent process ID */
    pid_t pid = getpid();
    pid_t parent_pid = getppid();

    /* Get the current time and format it */
    time_t current_time = time(NULL);
    char *start_time = ctime(&current_time);
    struct tm *timeinfo = localtime(&current_time);
    char formatted_start_time[80];
    strftime(formatted_start_time, sizeof(formatted_start_time), "%d-%m-%Y %H:%M:%S", timeinfo);

    /* Log start time */
    if (flag == 1) {
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\tParent Process ID: %d\tStart Time: %s\n", pid, parent_pid, formatted_start_time);
    /* Log end time */
    } else if (flag == 2) {
        bytes_written = snprintf(buffer, INBUF_SIZE, "\nProcess ID: %d\tParent Process ID: %d\tEnd Time: %s\n", pid, parent_pid, formatted_start_time);
    /* Log command details */
    } else {
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\tCommand Process ID: %d\t", pid, *command_pid);
        for (int i = 0; i < argc; i++) {
            size_t message_length = strlen(message[i]);
            strncat(buffer, message[i], INBUF_SIZE - buffer_length - 1);
            strncat(buffer, " ", INBUF_SIZE - buffer_length - message_length - 1);
        }
        strcat(buffer, "\n");
    }

    /* Check if the log message exceeds the remaining space in the shared memory */
    if (bytes_written >= remaining_space) {
        fprintf(stderr, "Log message exceeds shared memory size. Unable to write.\n");
        return;
    }

    /* Append the log message to the shared memory */
    strncat(&addr[index], buffer, remaining_space);
}
