/**
 * header 
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#define INBUF_SIZE 256

/* these should be the same as multishell.c */
#define MY_FILE_SIZE 1024
#define MY_SHARED_FILE_NAME "/sharedlogfile"

char *addr = NULL;
int fd = -1;

int initmem()
{
    fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0);
    if (fd < 0){
        perror("singleshell.c:fd:line31");
        exit(1);
    }
    addr = mmap(NULL, MY_FILE_SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == NULL){
        perror("singleshell.c:mmap:");
        close(fd);
        exit(1);
    }
    return 0;
}
/**
 * todo: you can start with myshellv3.c from lecture notes
 */
int main(int argc, char *argv[])
{
    initmem();

    char inbuf[INBUF_SIZE] = {'\0'};
    int lenbyte;
    while (1){
        write(1, "$ ", 2);
        if ((lenbyte = read(0, inbuf, 255)) <= 0){
            perror("input");
        } else{
            inbuf[lenbyte - 1] = '\0';
        }
        if (strncmp(inbuf, "exit", 4) == 0){
            exit(0);
        }
        pid_t child_pid = fork();

        if (child_pid == 0){
            int r = execl(inbuf,inbuf, NULL );
            if(r == -1){
                char command[256] = "/bin/";
                strncpy(command + 5, inbuf, 250);
                command[255] = '\0';
                r = execl(command, inbuf, NULL);
                if (r == -1)
                    perror("execl");
            }
            exit(0);
        }else if (child_pid > 0){
            wait(NULL);
        }else{
            perror("fork");
        }
    }
    // Unmap the shared memory
    munmap(addr, 1024);

    // Close the shared memory file
    close(fd);
}