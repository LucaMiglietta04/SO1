#include "my_cond.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void my_cond_init(my_cond_t *cond) {
    // Inicializa el semáforo en 0, cuando se la haga wait quedará lockeado.
    sem_init(&(cond->sem), 0, 0);
    pthread_mutex_init(&(cond->mutex_cond), NULL);
    cond->hilos_esperando = 0;
}

void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex) {
    pthread_mutex_lock(&(cond->mutex_cond));
    (cond->hilos_esperando)++;
    pthread_mutex_unlock(&(cond->mutex_cond));
    // suelta el mutex.
    pthread_mutex_unlock(mutex);
    // Baja el semáforo y espera un signal o un broadcast.
    sem_wait(&(cond->sem));
    // pide el mutex.
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t *cond) {
    // Levanta el semáforo.
    pthread_mutex_lock(&(cond->mutex_cond));
    if (cond->hilos_esperando > 0){
        (cond->hilos_esperando)--;
        sem_post(&(cond->sem));
    }
    pthread_mutex_unlock(&(cond->mutex_cond));
}

void my_cond_broadcast(my_cond_t *cond){
    pthread_mutex_lock(&(cond->mutex_cond));
    while(cond->hilos_esperando > 0){
        cond->hilos_esperando--;
        sem_post(&(cond->sem));
    }
    pthread_mutex_unlock(&(cond->mutex_cond));
}


void my_cond_destroy(my_cond_t *cond) {
    // Destruye el semáforo.
    sem_destroy(&(cond->sem));
    pthread_mutex_destroy(&(cond->mutex_cond));
}
























