#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 200000

unsigned int visitantes;
//pthread_mutex_t mutex_1 = PTRHEAD_MUTEX_INI;

void* molinete1(void* arg){
    for(int i = 0; i < N; i++)
        visitantes++;
}

void* molinete2(void* arg){
    for(int i = 0; i < N; i++)
        visitantes++;
}

int main(){
    pthread_t m1, m2;
    pthread_create(&m1, NULL, molinete1, NULL);
    pthread_create(&m2, NULL, molinete2, NULL);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("El número de visitantes fue %d\n", visitantes);

    return 0;
}
