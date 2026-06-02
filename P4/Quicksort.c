// Ej. 6 (Quicksort). Recordemos el algoritmo de ordenamiento Quicksort:
/* Particion de Lomuto, tomando el primer elemento como pivote */

// El tamanio exacto de cada tipo entero en C esta definido por la implementacion (i.e. el compilador). Para el GCC y
// Clang, en un sistema Linux de 64 bits, un long ocupa 64 bits.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include "timing.h"

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct{
    int pivot;
    int* arr;
} arg ;

int particionar(int a[], int N){
    int i, j = 0;
    int p = a[0];
    swap(&a[0], &a[N-1]);
    for (i = 0; i < N-1; i++){
        if (a[i] <= p)
        swap(&a[i], &a[j++]);
    }
    swap(&a[j], &a[N-1]);
    return j;
    }

void* qsort1(void* entry){
    arg* bloque = (arg*)entry;
    if ((bloque->pivot) < 2) 
    pthread_exit(NULL);
    int p = particionar(bloque->arr, bloque->pivot);
    pthread_t h1, h2;
    arg* bloque1 = malloc(sizeof(arg));
    arg* bloque2 = malloc(sizeof(arg));
    bloque1->arr = bloque->arr;
    bloque1->pivot = p;
    bloque2->arr = (bloque->arr) + p + 1;
    bloque2->pivot = (bloque->pivot) - p - 1;
    pthread_create(&h1, NULL, qsort1, (void*)bloque1);
    pthread_create(&h2, NULL, qsort1, (void*)bloque2);
    pthread_join(h1,NULL);
    pthread_join(h2,NULL);
}

void qsortMP(int a[], int N){
    if (N < 2)
    return;
    int p = particionar(a, N);
    #pragma omp parallel sections
    {
        #pragma omp section
        qsortMP(a, p);
        #pragma omp section
        qsortMP(a + p + 1, N - p - 1);        
    }
}
int main() {
    int datos1[10] = {9, 4, 7, 2, 1, 8, 5, 3, 6, 0};
    int datos2[10] = {9, 4, 7, 2, 1, 8, 5, 3, 6, 0};

    arg* argumento = malloc(sizeof(arg));
    argumento->arr = datos1;
    argumento->pivot = 10;

    printf("Probando Pthreads:\n");
    TIME_void(qsort1((void*)argumento), NULL);

    printf("\nProbando OpenMP:\n");
    TIME_void(qsortMP(datos2, 10), NULL);

    return 0;
}
// Dado que las llamadas recursivas para ordenar las “mitades” del arreglo son independientes, son un claro
// candidato para paralelizar.
// Como primer intento, escriba una version que use pthread_create para paralelizar las llamadas
// recursivas. Compare el rendimiento con la version secuencial para distintos tamanIos del array. ¿Hay
// alg´un problema? Explique.
// Escriba una versi´on que paralelice las llamadas usando sections de OpenMP. ¿Mejora la performance? ¿Cu´anto? Puede usar el servidor labdcc para probar en un quad-core.
// Escriba una versi´on usando tasks de OpenMP y mida el cambio en rendimiento.
