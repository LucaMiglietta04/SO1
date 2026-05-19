void rwlock_read_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    // AHORA el lector también se bloquea si hay escritores haciendo fila
    while (rw->escritores_escribiendo > 0 || rw->escritores_esperando > 0) {
        rw->lectores_esperando++;
        pthread_cond_wait(&rw->cond_lectores, &rw->mutex);
        rw->lectores_esperando--;
    }
    rw->lectores_leyendo++;
    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_write_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->escritores_escribiendo = 0;

    // Prioridad a escritores: si hay un escritor esperando, va él primero
    if (rw->escritores_esperando > 0) {
        pthread_cond_signal(&rw->cond_escritores);
    } else {
        pthread_cond_broadcast(&rw->cond_lectores);
    }
    pthread_mutex_unlock(&rw->mutex);
}