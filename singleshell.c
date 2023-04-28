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

int main(int argc, char **argv)
{
    char inbuf[INBUF_SIZE] = {'\0'};
    int lenbyte;
    initmem();
    while (1)
    {
        prompt();
        if ((lenbyte = read(0, inbuf, 255)) <= 0){
            perror("input");
            exit(1);
        }else{
            inbuf[lenbyte - 1] = '\0';
        }
        argv = split_string(inbuf,argv,&argc);
        argv = replace_variables(argv,argc);
        if(additional_functions(argv) == 0)
            exec(argv);
    }
    // Unmap the shared memory
    munmap(addr, 1024);

    // Close the shared memory file
    close(fd);
}
