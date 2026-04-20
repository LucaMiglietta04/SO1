#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 25000

unsigned int visitantes;
int cerrojo = 0;

void* molinete(void* arg){
    for(int i = 0; i < N; i++){
        while(!__sync_val_compare_and_swap(&cerrojo, 0, 1))
            {;};
        visitantes++;
        cerrojo = 0;
    }
}

int main(){
    pthread_t m1, m2, m3, m4;
    pthread_create(&m1, NULL, molinete, NULL);
    pthread_create(&m2, NULL, molinete, NULL);
    pthread_create(&m3, NULL, molinete, NULL);
    pthread_create(&m4, NULL, molinete, NULL);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);
    pthread_join(m3, NULL);
    pthread_join(m4, NULL);

    printf("El número de visitantes fue %d\n", visitantes);

    return 0;
}
