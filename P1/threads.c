#include <pthread.h>
#include <stdio.h>

static void* printeo(void* arg){
    printf("Hola mundo!\n");
    return NULL;
}

int main(){
    pthread_t hilo_id;

    pthread_create(&hilo_id, NULL, printeo, NULL);

    pthread_join(hilo_id, NULL);

    printf("termino!!!!\n");
    return 0;
}