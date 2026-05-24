#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "barreras.h"

void barrier_init(struct barrier *b, int n){
    b->hilos_esperando = 0;
    b->hilos_totales = n;
    b->ciclo = 0;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->cv, NULL);
}

void barrier_wait(struct barrier *b){
    
    pthread_mutex_lock(&(b->lock));

    int ciclo_actual = b->ciclo;
    b->hilos_esperando++;

    if (b->hilos_esperando == b->hilos_totales){

        b->hilos_esperando = 0;
//        b->ciclo++;
        pthread_cond_broadcast(&(b->cv));

    } else {
        while(ciclo_actual == b->ciclo)
            pthread_cond_wait(&(b->cv), &(b->lock));
    }
    
    pthread_mutex_unlock(&(b->lock));
}
