#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int* segundos = (int*) argv[3];
    for(int i = 0; i < 10; i++){
        execl(argv[1], argv[2], (char*)NULL);
        sleep(*segundos);
    }
    return 0;
}