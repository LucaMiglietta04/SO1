#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    
    int pipefd[2];
    pipe(pipefd);
    char buff;
    char* str = "Luca y Luisi son plomeros";

    pid_t pid = fork();

    if(pid == 0){
        close(pipefd[1]);
        while(read(pipefd[0], &buff, 1) > 0)
            printf("%c", buff);
        printf("\n");
        close(pipefd[0]);
        exit(1);
    } else {
        close(pipefd[0]);
        write(pipefd[1], str, strlen(str));
        close(pipefd[1]);
        wait(0);
    }

    return 0;
}