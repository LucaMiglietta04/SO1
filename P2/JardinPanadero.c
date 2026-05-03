#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define N 3                    // Número de hilos (molinetes)
#define V_POR_HILO 10000        

/* Variables Globales */
int numero[N] = {0};
bool eligiendo[N] = {false};

int visitantes = 0; // Recurso compartido

int obtener_max() {
    int max = 0;
    for (int i = 0; i < N; i++) {
        if (numero[i] > max) max = numero[i];
    }
    return max;
}

void lock(int i) {
    // 1. Tomar un número de turno
    eligiendo[i] = true;
    __sync_synchronize(); // Memory barrier (como el mfence que mencionaste)
    
    numero[i] = 1 + obtener_max();
    
    __sync_synchronize();
    eligiendo[i] = false;

    // 2. Esperar el turno
    for (int j = 0; j < N; j++) {
        // Esperar si el hilo j está eligiendo número
        while (eligiendo[j]);

        // Esperar mientras el hilo j tenga un número menor (o igual pero con ID menor)
        while (numero[j] != 0 && 
              (numero[j] < numero[i] || (numero[j] == numero[i] && j < i))) {
            // Busy waiting
        }
    }
}

void unlock(int i) {
    __sync_synchronize();
    numero[i] = 0;
}

void* molinete(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < V_POR_HILO; i++) {
        lock(id);
                
        visitantes++; /* SECCIÓN CRÍTICA */
        
        unlock(id);
    }
    return NULL;
}

int main() {
    pthread_t hilos[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, molinete, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Total de visitantes: %d\n", visitantes);
    return 0;
}