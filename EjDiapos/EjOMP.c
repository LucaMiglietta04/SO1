#include <omp.h>
#include <stdio.h>

int main(){
int id, nt;

    #pragma omp parallel
    {
        id = omp_get_thread_num();
        nt = omp_get_num_threads();
        printf("id: %d nt: %d\n", id, nt);
    }
    
    printf("after parallel id: %d nt: %d\n", id, nt);
    return 0;
}