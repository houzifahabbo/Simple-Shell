/**
 * header
 *
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
fd = shm_open(MY_SHARED_FILE_NAME,
                  O_CREAT | O_RDWR | O_TRUNC, 0666);    
    //fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0); 
    if (fd < 0)
    {
        perror("singleshell.c:fd:line31");
        exit(1);
    }
    if (ftruncate(fd, MY_FILE_SIZE) == -1) {
        perror("singleshell.c:ftruncate:");
        close(fd);
        exit(1);
    }
    addr = mmap(NULL, MY_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == NULL)
    {
        perror("singleshell.c:mmap:");
        close(fd);
        exit(1);
    }
    return 0;
}

void prompt(){
    char cwd[256];
    char prompt [1024] = {'\0'};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(1);
    }
    snprintf(prompt,sizeof(prompt),"\033[0;34m%s\033[0m:\033[0;32m%s\033[0m$ ",getenv("USER"),cwd);
    write(1, prompt , (strlen(prompt)+1));
}

void log_message(char *message,pid_t * command_pid ,int flag){
    char buffer[INBUF_SIZE] = {'\0'};
    int bytes_written,remaining_space = MY_FILE_SIZE - strlen(addr) - 1;
    pid_t pid = getpid(),parent_pid = getppid();
    time_t current_time = time(NULL);
    char *start_time = ctime(&current_time);
    struct tm *timeinfo = localtime(&current_time);
    char formated_start_time[80];
    strftime(formated_start_time, sizeof(formated_start_time), "%d-%m-%Y %H:%M:%S", timeinfo);
    if(flag == 1)
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\nParent Process ID: %d\nStart Time: %s\n", pid, parent_pid,formated_start_time);
    else if(flag == 2)
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\tCommand Process ID: %d\t%s\nEnd Time: %s\n", pid,*command_pid,message,formated_start_time);
    else
        bytes_written = snprintf(buffer, INBUF_SIZE, "Process ID: %d\tCommand Process ID: %d\t%s\n", pid,*command_pid,message);

    if (bytes_written >= remaining_space) {
        fprintf(stderr, "Log message exceeds shared memory size. Unable to write.\n");
        return;
    }
    strncat(addr, buffer, remaining_space);
}

int main(int argc, char **argv)
{
    char inbuf[INBUF_SIZE] = {'\0'};
    int lenbyte;
    pid_t command_pid = 0;
    initmem();
    log_message(NULL,&command_pid,1);
    while (1)
    {
        prompt();
        if ((lenbyte = read(0, inbuf, INBUF_SIZE - 1)) <= 0)
        {
            perror("input");
            exit(1);
        }
        else
        {
            inbuf[lenbyte - 1] = '\0';
        }     
        argv = split_string(inbuf, argv, &argc);
        argv = replace_variables(argv, argc);
        int x = additional_functions(argv,argc,&command_pid);
        if (x==0)
            command_pid = exec(argv);
        else if(x==2){
            log_message(inbuf,&command_pid,2);
            free(argv);
            break;
        }
        log_message(inbuf,&command_pid,0);
        free(argv);
    }

    // Unmap the shared memory
    munmap(addr, MY_FILE_SIZE);
    // Close the shared memory file
    close(fd);
    
    exit(0);
}