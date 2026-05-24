#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "rwlock.h"

#define M 5
#define N 5
#define ARRLEN 10240
int arr[ARRLEN];

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

int main()
{
    rwlock_init(&lock);
    pthread_t lectores[M], escritores[N];
    int i;
    for (i = 0; i < M; i++)
    pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}