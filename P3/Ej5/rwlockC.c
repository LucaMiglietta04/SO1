// Agregamos un flag a la estructura en rwlock.h: int turno_escritores; (0 para lectores, 1 para escritores)

void rwlock_read_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    // Espera si hay un escritor activo, o si hay escritores esperando Y es el turno de ellos
    while (rw->escritores_escribiendo > 0 || (rw->escritores_esperando > 0 && rw->turno_escritores)) {
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
    if (rw->lectores_leyendo == 0) {
        rw->turno_escritores = 1; // Próximo turno para escritores
        pthread_cond_signal(&rw->cond_escritores);
    }
    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_write_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    // Espera si hay lectores o escritores activos, o si hay lectores esperando y es su turno
    while (rw->lectores_leyendo > 0 || rw->escritores_escribiendo > 0 || (rw->lectores_esperando > 0 && !rw->turno_escritores)) {
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
    
    // Alternamos equitativamente
    if (rw->lectores_esperando > 0) {
        rw->turno_escritores = 0;
        pthread_cond_broadcast(&rw->cond_lectores);
    } else if (rw->escritores_esperando > 0) {
        rw->turno_escritores = 1;
        pthread_cond_signal(&rw->cond_escritores);
    }
    pthread_mutex_unlock(&rw->mutex);
}