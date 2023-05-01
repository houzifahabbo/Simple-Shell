#include "main.h"
#define INBUF_SIZE 256
#define MY_FILE_SIZE 1024

void log_message(char **message,pid_t * command_pid ,int flag,int argc,int index ,char * addr){
    char buffer[INBUF_SIZE] = {'\0'};
    size_t buffer_length = strlen(buffer);
    int bytes_written,remaining_space = MY_FILE_SIZE - strlen(addr) - 1;
    pid_t pid = getpid(),parent_pid = getppid();
    time_t current_time = time(NULL);
    char *start_time = ctime(&current_time);
    struct tm *timeinfo = localtime(&current_time);
    char formated_start_time[80];
    strftime(formated_start_time, sizeof(formated_start_time), "%d-%m-%Y %H:%M:%S", timeinfo);
    if(flag == 1)
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\tParent Process ID: %d\tStart Time: %s\n", pid, parent_pid,formated_start_time);
    else if(flag == 2)
        bytes_written = snprintf(buffer, INBUF_SIZE, "\nProcess ID: %d\tParent Process ID: %d\tEnd Time: %s\n",pid, parent_pid,formated_start_time);
    else{
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\tCommand Process ID: %d\t", pid,*command_pid);
        for (int i = 0; i < argc; i++)
        {
            size_t message_length = strlen(message[i]);
            strncat(buffer, message[i], INBUF_SIZE - buffer_length - 1);
            strncat(buffer, " ", INBUF_SIZE - buffer_length - message_length - 1);
        }
        strcat(buffer,"\n");
    }

    if (bytes_written >= remaining_space) {
        fprintf(stderr, "Log message exceeds shared memory size. Unable to write.\n");
        return;
    }
    strncat(&addr[index], buffer, remaining_space);
}