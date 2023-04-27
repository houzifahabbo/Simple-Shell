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
    fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0);
    if (fd < 0)
    {
        perror("singleshell.c:fd:line31");
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


int main(int argc, char **argv)
{
    initmem();
    char inbuf[INBUF_SIZE] = {'\0'};
    int lenbyte;
    while (1)
    {
        write(1, "$", 2);
        if ((lenbyte = read(0, inbuf, 255)) <= 0)
        {
            perror("input");
            exit(1);
        }
        else
        {
            inbuf[lenbyte - 1] = '\0';
        }
        if (strncmp(inbuf, "exit", 4) == 0)
        {
            exit(0);
        }
        pid_t child_pid = fork();
        if (child_pid == 0)
        {
            argv = split_string(inbuf,argv);
            exec(argv);
            exit(0);
        }
        else if (child_pid > 0){
            waitpid(child_pid, NULL, 0);
        }else{
            perror("fork");
            exit(1);
        }
    }
    // Unmap the shared memory
    munmap(addr, 1024);

    // Close the shared memory file
    close(fd);
} 