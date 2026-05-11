#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void){
    int pid;
    pid = fork();
    if (pid == 0){
        printf("Soy el hijo\n");
        sleep(10);
        execl("./ejecutable", "ejecutable", (char *)NULL);
//        exit(EXIT_SUCCESS);
    } else {
        printf("El hijo tiene pid %d\n", pid);
        sleep(10);
    }
}
