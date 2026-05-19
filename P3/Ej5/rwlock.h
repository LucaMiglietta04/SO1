#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond_lectores;
    pthread_cond_t cond_escritores;
    
    int lectores_leyendo;   // Cantidad de lectores dentro de la RC
    int escritores_escribiendo; // 0 o 1
    int lectores_esperando;  // Lectores bloqueados esperando entrar
    int escritores_esperando; // Escritores bloqueados esperando entrar
} rwlock_t;

void rwlock_init(rwlock_t *rw);
void rwlock_read_lock(rwlock_t *rw);
void rwlock_read_unlock(rwlock_t *rw);
void rwlock_write_lock(rwlock_t *rw);
void rwlock_write_unlock(rwlock_t *rw);

#endif