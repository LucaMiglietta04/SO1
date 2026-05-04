#define _XOPEN_SOURCE 600 // Por culpa de esta compu petera
#include <pthread.h>
#include <stdio.h>

pthread_barrier_t barrier;

void* thread_func(void* arg){
    printf("Hilo %ld antes de la barrera\n", (long)arg);
    pthread_barrier_wait(&barrier);
    printf("Hilo %ld después de la barrera\n", (long)arg);
}

int main(){
    pthread_t threads[3];
    pthread_barrier_init(&barrier, NULL, 3);

    for(long i = 0; i<3; i++){
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    for(int i = 0; i<3; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}