#ifndef BARRERAS_H
#define BARRERAS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

struct barrier {
    pthread_mutex_t lock;
    pthread_cond_t cv;
    int hilos_esperando;
    int hilos_totales;
    int ciclo; // necesario porq si no se adelanta otro hilo antes de que se despierte el resto
                      // y se hace deadlock
};

void barrier_init(struct barrier *b, int n);

void barrier_wait(struct barrier *b);

#endif