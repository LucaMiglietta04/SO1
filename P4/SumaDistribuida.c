/*Ej. 8 (Suma Distribuida). Implemente en MPI un programa distribuido 
que compute la suma de un array distribuyendo segmentos del mismo. 
Su solucion debe ser robusta si var´ıa el tama˜no del array y/o la
cantidad de procesos involucrados.
Corra en programa con distintas cantidad de procesos: 4, 8, y 16.*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM 12

int main(int argc, char** argv) {
    int my_id, num_procs;
    int root_rank = 0;
    int arreglo[TAM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int suma = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int elemProc = TAM / num_procs; // calculo cuántos elementos operará cada proceso
    int local_data[elemProc]; 

    //  int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
    MPI_Scatter(arreglo, elemProc, MPI_INT, local_data, elemProc, MPI_INT,root_rank, MPI_COMM_WORLD);

    int sumaLocal = 0;
    for (int i = 0; i < elemProc; i++) {
        sumaLocal += local_data[i];
    }

    // int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
    MPI_Reduce(&sumaLocal, &suma, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);

    if (my_id == root_rank) {
        printf("La suma total distribuida del arreglo es: %d\n", suma);
    }
    
    MPI_Finalize();
    return 0;
}