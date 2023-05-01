/*
 *    File: multishell.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helani, Nour Chami, Muslim Umalatov
 * Purpose: 
 */

#include "main.h"

/* these should be the same as multishell.c */
#define MY_FILE_SIZE 1024
#define MY_SHARED_FILE_NAME "/sharedlogfile"

#define MAX_SHELL 10
#define DEFAULT_NSHELL 2
char *addr = NULL; /* mmap address */
int fd = -1;       /* fd for shared file object */

int initmem()
{
    fd = shm_open(MY_SHARED_FILE_NAME,
                  O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd < 0) {
        perror("multishell.c:open file:");
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

void save_to_file(){
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);
    char file_name[50];
    strftime(file_name, sizeof(file_name), "shelllog-[%d-%m-%Y %H:%M:%S].txt", time_info);
    
    FILE *log_file = fopen(file_name, "w");
    if (log_file == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(log_file, "%s", addr);
    fclose(log_file);
}

/**
 * todo, you can create multiple function, variables, etc..
 * */

int main(int argc, char **argv)
{
    int nshell = DEFAULT_NSHELL;
    
    if (argc > 1) {
        nshell = atoi(argv[1]);
        if (nshell <= 0 || nshell > MAX_SHELL) {
            fprintf(stderr, "Invalid number of shells. Setting to default: %d\n", DEFAULT_NSHELL);
            nshell = DEFAULT_NSHELL;
        }
    }
    initmem();
    pid_t child_pids[MAX_SHELL];

    for (int i = 0; i < nshell; i++) {
        pid_t child_pid = fork();
        if(child_pid == 0){
            char title[20];
            snprintf(title, sizeof(title), "singleshell %d", i+1);
            if (execlp("xterm", "xterm", "-fa", "Ubuntu Mono" ,"-fs" ,"15","-T",title ,"-e", "./singleshell", "singleshell", NULL) == -1) {
                perror("execlp");
                exit(1);
            }
            exit(0);
        }else if (child_pid >0){
            child_pids[i] = child_pid;
        }else{
            perror("fork");
            exit(1);
        }
    }
    for (int i = 0; i < nshell; i++) {
        waitpid(child_pids[i], NULL, 0);
        save_to_file();
    }
    

    /* unlink mmap */
    munmap(addr, 1024);

    /* close the shared memory file */
    close(fd);

    return 0;
}
