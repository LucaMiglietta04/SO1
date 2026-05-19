#include "rwlock.h"

void rwlock_init(rwlock_t *rw) {
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->cond_lectores, NULL);
    pthread_cond_init(&rw->cond_escritores, NULL);
    rw->lectores_leyendo = 0;
    rw->escritores_escribiendo = 0;
    rw->lectores_esperando = 0;
    rw->escritores_esperando = 0;
}

void rwlock_read_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    // Un lector solo espera si hay un escritor escribiendo en este momento
    while (rw->escritores_escribiendo > 0) {
        rw->lectores_esperando++;
        pthread_cond_wait(&rw->cond_lectores, &rw->mutex);
        rw->lectores_esperando--;
    }
    rw->lectores_leyendo++;
    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_read_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->lectores_leyendo--;
    // Si soy el último lector, le aviso a los escritores que esperan
    if (rw->lectores_leyendo == 0) {
        pthread_cond_signal(&rw->cond_escritores);
    }
    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_write_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    // Un escritor espera si hay alguien leyendo u otro escribiendo
    while (rw->lectores_leyendo > 0 || rw->escritores_escribiendo > 0) {
        rw->escritores_esperando++;
        pthread_cond_wait(&rw->cond_escritores, &rw->mutex);
        rw->escritores_esperando--;
    }
    rw->escritores_escribiendo = 1;
    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_write_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->escritores_escribiendo = 0;
    
    // Como los lectores tienen prioridad, despertamos a TODOS los lectores primero.
    // Si no hay ninguno, despertamos a un escritor.
    if (rw->lectores_esperando > 0) {
        pthread_cond_broadcast(&rw->cond_lectores);
    } else {
        pthread_cond_signal(&rw->cond_escritores);
    }
    pthread_mutex_unlock(&rw->mutex);
}