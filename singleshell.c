/*
 *    File: singleshell.c
 * Project: system-programming-project-1
 * Authors: Hozaifah Habbo, Ola Helany, Nour Chami, Muslim Umalatov
 * Purpose: 
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

char* prompt(){
    char cwd[256]={'\0'};
    char prompt[1024]={'\0'};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(1);
    }
    snprintf(prompt, sizeof(prompt),
            "\033[0;36m%s\033[0;37m:\033[0;32m%s\033[0;37m$ \033[0m",
            getenv("USER"), cwd);
    return strdup(prompt);
}

char* history(char **argv, char **history_str, int history_str_size) {
    if (argv[1] == NULL) {
        for (int i = 0; i < history_str_size; i++) {
            fprintf(stdout, "%d:  %s\n", i + 1, history_str[i]);
            fflush(stdout);
        }
        return NULL;
    } else {
        int index = atoi(argv[1]);
        index--;
        if (index >= 0 && index < history_str_size){
            if(strcmp(history_str[index],"null")==0)
                return NULL;
            else
                return history_str[index];
        }
    }
    return NULL;
}


int main(int argc, char **argv)
{
    char *inbuf;
    char *history_str[10]={"null","null","null","null","null","null","null","null","null","null"};
    int i=0;
    int flag=0;
    int history_str_size = sizeof(history_str)/sizeof(history_str[0]);
    pid_t command_pid = getpid();
    initmem();
    log_message(NULL,&command_pid,1,argc,i,addr);
    while (1)
    {
        command_pid = getpid();
        char *prompt_txt= prompt();
        if(flag == 0){
            inbuf = malloc(INBUF_SIZE);
            if (inbuf == NULL) {
                perror("malloc");
                exit(1);
            }
            strcpy(inbuf, readline(prompt_txt));
        }else{ 
            flag = 0;
            char *history_input = history(argv,history_str,history_str_size);
            if(history_input != NULL){
                strcat(prompt_txt,history_input);
                snprintf(inbuf,sizeof(inbuf),"%s %s",history_input,readline(prompt_txt));
            }else{
                continue;
            }
        }
        if (inbuf) {
            add_history(inbuf);
            if(strncmp("history",inbuf,7)!=0){
                history_str[i] = strdup(inbuf);
            }
        }
        argv = split_string(inbuf, argv, &argc);
        if(argv[0] == NULL){
            continue;
        }
        argv = replace_variables(argv, argc);
        int x = additional_functions(argv, argc, &command_pid);

        if (x == 0)
            command_pid = exec(argv);
        else if(x == 2){
            flag = 1;
            continue;
        }else if(x == 3){
            log_message(argv,&command_pid,2,argc,i,addr);
            free(argv);
            break;
        }
        log_message(argv,&command_pid,0,argc,i,addr);
        if(i==10)
            i=0;
        else
            i++;
        free(argv);
        free(inbuf);
    }

    /* Unmap the shared memory */
    munmap(addr, MY_FILE_SIZE);
    
    /* Close the shared memory file */
    close(fd);
    
    exit(0);
}