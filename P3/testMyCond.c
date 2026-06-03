#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "my_cond.h"  // Asegurate de que tu cabecera se llame así

#define ITERACIONES 5
#define NUM_CONSUMIDORES 3

// Variables globales para el estado del test
pthread_mutex_t mutex_compartido = PTHREAD_MUTEX_INITIALIZER;
my_cond_t cond_producido;
my_cond_t cond_consumido;

int buffer_item = -1; // -1 significa vacío
int produccion_terminada = 0;

// Hilo Productor: Genera elementos y avisa con my_cond_signal
void* productor(void* arg) {
    for (int i = 0; i < ITERACIONES * NUM_CONSUMIDORES; i++) {
        pthread_mutex_lock(&mutex_compartido);
        
        // Si el búfer está lleno, espera a que se consuma
        while (buffer_item != -1) {
            printf("[Productor] Búfer lleno. Esperando...\n");
            my_cond_wait(&cond_consumido, &mutex_compartido);
        }
        
        // Producir elemento
        buffer_item = i + 1;
        printf("[Productor] Producido item: %d\n", buffer_item);
        
        // Despierta a UN consumidor que esté esperando
        my_cond_signal(&cond_producido);
        
        pthread_mutex_unlock(&mutex_compartido);
        usleep(100000); // Pequeña pausa simulada
    }
    
    // Avisar que terminó la producción total
    pthread_mutex_lock(&mutex_compartido);
    produccion_terminada = 1;
    printf("[Productor] Producción terminada. Despertando a todos los hilos restantes...\n");
    my_cond_broadcast(&cond_producido); // Testeo de broadcast
    pthread_mutex_unlock(&mutex_compartido);
    
    return NULL;
}

// Hilos Consumidores: Esperan elementos y vacían el búfer
void* consumidor(void* arg) {
    long id = (long)arg;
    
    while (1) {
        pthread_mutex_lock(&mutex_compartido);
        
        // Espera mientras el búfer esté vacío y no haya terminado la producción
        while (buffer_item == -1 && !produccion_terminada) {
            printf("[Consumidor %ld] Búfer vacío. Durmiendo en my_cond_wait...\n", id);
            my_cond_wait(&cond_producido, &mutex_compartido);
        }
        
        // Si terminó la producción y ya no quedan elementos, sale
        if (buffer_item == -1 && produccion_terminada) {
            pthread_mutex_unlock(&mutex_compartido);
            break;
        }
        
        // Consumir el elemento
        printf("[Consumidor %ld] Consumió item: %d\n", id, buffer_item);
        buffer_item = -1; // Vaciar búfer
        
        // Avisar al productor que el búfer está libre
        my_cond_signal(&cond_consumido);
        
        pthread_mutex_unlock(&mutex_compartido);
        usleep(200000); // Simula tiempo de procesamiento del dato
    }
    
    printf("[Consumidor %ld] Terminó su ejecución.\n", id);
    return NULL;
}

int main() {
    pthread_t th_productor;
    pthread_t th_consumidores[NUM_CONSUMIDORES];
    
    printf("=== Iniciando Test de Variables de Condición Caseras ===\n");
    
    // 1. Probar Inicialización
    my_cond_init(&cond_producido);
    my_cond_init(&cond_consumido);
    
    // 2. Crear hilos concurrentes
    pthread_create(&th_productor, NULL, productor, NULL);
    for (long i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_create(&th_consumidores[i], NULL, consumidor, (void*)i);
    }
    
    // 3. Esperar a que terminen todos
    pthread_join(th_productor, NULL);
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(th_consumidores[i], NULL);
    }
    
    // 4. Probar Destrucción
    my_cond_destroy(&cond_producido);
    my_cond_destroy(&cond_consumido);
    
    printf("=== Test Finalizado Exitosamente sin Deadlocks ===\n");
    return 0;
}