#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "barreras.h"

#define N 1000
#define W 4      
#define ITERS 10

float arr1[N], arr2[N];

struct barrier barrera;

static inline int min(int a, int b) { return a < b ? a : b; }

static inline int cut(int n, int i, int m) { 
    return i * (n/m) + min(i, n % m); 
}

void calor(float *arr, int lo, int hi, float *arr2_ptr) { 
    int i;
    for (i = lo; i < hi; i++) { 
        float m = arr[i]; 
        float l = i > 0 ? arr[i-1] : m; 
        float r = i < N-1 ? arr[i+1] : m; 
        arr2_ptr[i] = m + (l - m)/1000.0 + (r - m)/1000.0;
    } 
}

void * thr(void *arg) { 
    long id = (long)arg;
    int lo = cut(N, id, W);
    int hi = cut(N, id+1, W); 
    int i;

    for (i = 0; i < ITERS; i++) { 
        calor(arr1, lo, hi, arr2);
        printf("hilo %ld reparte calor 1\n", id);
        
        barrier_wait(&barrera);

        calor(arr2, lo, hi, arr1);
        printf("hilo %ld reparte calor 2\n", id);
        
        barrier_wait(&barrera);
    } 
    return NULL;
}

int main() {
    pthread_t hilos[W];

    for(int i = 0; i < N; i++) arr1[i] = 25.0;
    arr1[N/2] = 80.0;

    barrier_init(&barrera, W);

    for (long i = 0; i < W; i++) {
        pthread_create(&hilos[i], NULL, thr, (void*)i);
    }

    for (int i = 0; i < W; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Fin hilitos\n");
    return 0;
}