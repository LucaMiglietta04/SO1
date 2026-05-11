#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int s){
    printf("La señal fue recibida\n");
}

int main(){

    int rc;
    struct sigaction sigact = {
        .sa_handler = handler,
    };


    rc = sigaction(SIGUSR1, &sigact, NULL);
    if (rc < 0)
        exit(-1);
    __pid_t pid1 = fork();
    if (pid1 == 0){
        exit(1);
    } else {
        kill(pid1, SIGUSR1);
        wait(0);
        printf("El padre vino despues\n");
    }
    return 0;
}