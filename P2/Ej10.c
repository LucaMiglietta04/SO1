#include <pthread.h>
#include <stdio.h>

#define N 100

int arreglo[N];
int total = 0;
pthread_mutex_t cerrojo = PTHREAD_MUTEX_INITIALIZER;

void* sumar(void* arg){
    int inicio = *(int*)arg;
    int final = inicio + (N/2);
    
    for(int i = inicio; i < final; i++){
        pthread_mutex_lock(&cerrojo);
        total += arreglo[i];
        pthread_mutex_unlock(&cerrojo);
    }
    return NULL;
}

int main(){
    pthread_t m1, m2;
    int arg1 = 0, arg2 = N/2;

    for(int i = 0; i < N; i++)
        arreglo[i] = 1;

    pthread_create(&m1, NULL, sumar, &arg1);
    pthread_create(&m2, NULL, sumar, &arg2);

    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("Suma: %d\n", total);
    return 0;
}