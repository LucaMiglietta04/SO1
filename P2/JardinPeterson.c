#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 200000

unsigned int visitantes;
int flags[2] = {0};
int turn = 0;

void* molinete1(void* arg){
    
for(int i = 0; i < N; i++){
    // lock()
   flags[0] = 1;
   turn = 2;
   while (flags[1] && (turn == 2))
       ;
    /* sección crítica */
    visitantes++;
    printf("m1: %d\n", visitantes);

    /* unlock */
    flags[0]=0;
    }
}

void* molinete2(void* arg){
    
for(int i = 0; i < N; i++){
// lock()
   flags[1] = 1;
   turn = 1;
   while (flags[0] && (turn == 1))
       ;
/* sección crítica */
    visitantes++;
    printf("m2: %d\n", visitantes);

/* unlock */
   flags[1]=0;
    }
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
