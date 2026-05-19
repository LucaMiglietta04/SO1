rwlock_t lock; // Instancia de nuestro lock genérico

void *escritor(void *arg) {
    int i;
    int num = (int)(long)arg;
    while (1) {
        sleep(random() % 3);
        
        rwlock_write_lock(&lock); // <--- Bloqueo de Escritura
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++) arr[i] = num;
        rwlock_write_unlock(&lock); // <--- Desbloqueo de Escritura
    }
    return NULL;
}

void *lector(void *arg) {
    int v, i;
    int num = (int)(long)arg;
    while (1) {
        sleep(random() % 3);
        
        rwlock_read_lock(&lock); // <--- Bloqueo de Lectura
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v) break;
        }
        if (i < ARRLEN) printf("Lector %d, error de lectura\n", num);
        else printf("Lector %d, dato %d\n", num, v);
        rwlock_read_unlock(&lock); // <--- Desbloqueo de Lectura
    }
    return NULL;
}