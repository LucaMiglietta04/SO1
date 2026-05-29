#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 500000000

int main(){
    int* arr = malloc(sizeof(int)*N);

    #pragma omp parallel for
    for(int i = 0; i < N; i++)
        arr[i] = (i % 1000) + 5;

    int minimo = arr[0];

    #pragma omp parallel for reduction(min: minimo)
    for(int i = 1; i < N; i++){
        if (arr[i] < minimo)
            minimo = arr[i];
    }

    printf("Minimo = %d\n", minimo);
    free(arr);

    return 0;
}