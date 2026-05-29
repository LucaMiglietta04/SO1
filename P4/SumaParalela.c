#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 500000000

int main(){
    double* arr = malloc(sizeof(double)*N);

    #pragma omp parallel for
    for(int i = 0; i < N; i++)
        arr[i] = 1.0;

    double suma = 0;

    #pragma omp parallel for reduction(+: suma)
    for(int i = 0; i < N; i++){
        suma = suma + arr[i];
    }

    printf("Suma total = %lf\n", suma);
    free(arr);

    return 0;
}