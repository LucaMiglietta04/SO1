// Ej. 5 (Multiplicacion de Matrices). Implemente en OpenMP la multiplicacion de dos matrices en
// paralelo. Una versoion secuencial es:

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "timing.h"

#define N 1000
int A[N][N], B[N][N], C[N][N];

int mult(int A[N][N], int B[N][N], int C[N][N]){
    int i, j, k;
    #pragma omp parallel for private(j, k)
    for (i = 0; i < N; i++)
    for (k = 0; k < N; k++)
    for (j = 0; j < N; j++)
               C[i][j] += A[i][k] * B[k][j];
    return 0;
}

int multSINMP(int A[N][N], int B[N][N], int C[N][N]){
    int i, j, k;
   // #pragma omp parallel for
    for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
    for (k = 0; k < N; k++)
    C[i][j] += A[i][k] * B[k][j];
    return 0;
}


int main() {
    float tiempo_transcurrido1, tiempo_transcurrido2;
    long resultado1, resultado2;
    int i, j;
    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
        A[i][j] = random() % 1000;
        B[i][j] = random() % 1000;
        
        }
    }
    
    // Mide la ejecución y guarda el "Wall time" en 'tiempo_transcurrido'
    resultado1 = TIME(mult(A, B, C), &tiempo_transcurrido1);
    resultado2 = TIME(multSINMP(A, B, C), &tiempo_transcurrido2);
  

    printf("El resultado con OMP fue %ld y tardó %f segundos.\n", resultado1, tiempo_transcurrido1);
    printf("El resultado sin OMP fue %ld y tardó %f segundos.\n", resultado2, tiempo_transcurrido2);
    return 0;
}

// a) Compare la performance con la solucion secuencial para matrices cuadradas de tamano 200x200,
// 500x500 y 1000x1000. ¿Que relacion aproximada puede inferir entre los tiempos en uno y otro caso?
// b) Si se cambia el orden de los ındices, ¿se puede mejorar el rendimiento? ¿Por que? SI
// c) Si tuviese que computar la multiplicacion de A × BT
// , ¿se puede mejorar el rendimiento? ¿Por que?