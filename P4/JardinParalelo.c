#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <omp.h>

#define VisitPorHilo 40000
#define MaxThreads 16

/* Variables Globales */
int numero[MaxThreads] = {0};
bool eligiendo[MaxThreads] = {false};

int visitantes = 0;

int obtener_max(int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        if (numero[i] > max) max = numero[i];
    }
    return max;
}

void lock(int i, int num_threads) {
    // 1. Tomar un número de turno
    eligiendo[i] = true;
    __sync_synchronize(); // (mfence)
    
    numero[i] = 1 + obtener_max(num_threads);
    
    __sync_synchronize();
    eligiendo[i] = false;

    // 2. Esperar el turno
    for (int j = 0; j < num_threads; j++) {
        while (eligiendo[j]);

        while (numero[j] != 0 && 
              (numero[j] < numero[i] || (numero[j] == numero[i] && j < i))) {
        }
    }
}

void unlock(int i) {
    __sync_synchronize();
    numero[i] = 0;
}


int main() {

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        for (int i = 0; i < VisitPorHilo; i++) {
            lock(id, num_threads);
                    
            visitantes++;
            
            unlock(id);
        }
    }   

    printf("Total de visitantes: %d\n", visitantes);
    return 0;
}
