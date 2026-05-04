#include <omp.h>
#include <stdio.h>

int main(){
    int a[100000];

    #pragma omp parallel
    {
        int contador = 0;
        #pragma omp for
        for (int i = 0; i < 100000; i++){
            a[i] = 2 * i;
            contador++;
        }
        printf("Iteraciones: %d\n", contador);
    }
}