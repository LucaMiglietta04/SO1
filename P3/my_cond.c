#include "my_cond.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void my_cond_init(my_cond_t *cond) {
    // Inicializa el semáforo en 0, cuando se la haga wait quedará lockeado.
    sem_init(&cond->signals, 0, 0);
    sem_init(&cond->lock, 0,1); 
    sem_init(&cond->check, 0,0); 
    cond->hilos_esperando = 0;
}

void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex) {
    sem_wait(&cond->lock);
    (cond->hilos_esperando)++;
    sem_post(&cond->lock);
    // suelta el mutex.
    pthread_mutex_unlock(mutex);
    // Baja el semáforo y espera un signal o un broadcast.
    sem_wait(&cond->signals);
    // Enviamos la señal de salida.
    sem_post(&cond->check);
    // pide el mutex.
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t *cond) {
    // Levanta el semáforo.
    sem_wait(&cond->lock);
    if (cond->hilos_esperando > 0){
        cond->hilos_esperando--;
        sem_post(&cond->signals);
        sem_wait(&cond->check);
    }
    sem_post(&cond->lock);
}

void my_cond_broadcast(my_cond_t *cond){
    sem_wait(&cond->lock);
    for(int i = 0; i < cond->hilos_esperando; i++)
        sem_post(&cond->signals);
    while(cond->hilos_esperando > 0){
        cond->hilos_esperando--;
        sem_wait(&cond->check);
    }
    sem_post(&cond->lock);
}


void my_cond_destroy(my_cond_t *cond) {
    // Destruye el semáforo.
    sem_destroy(&cond->signals);
    sem_destroy(&cond->lock);
}























