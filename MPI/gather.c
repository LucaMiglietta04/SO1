#include <stdio.h>
#include <string.h>
#include <mpi.h>

void main( int argc, char** argv){
    int root_rank = 0;
    int my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int my_value = my_rank * 100;
    printf("Process %d, my value = %d.\n", my_rank, my_value);
    if(my_rank == root_rank) {
    int buffer[4];
    MPI_Gather(&my_value, 1, MPI_INT, buffer, 1, MPI_INT, root_rank,
    MPI_COMM_WORLD);
    printf("Values collected on process %d: %d, %d, %d, %d.\n", my_rank,
    buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    else {
    MPI_Gather(&my_value, 1, MPI_INT, NULL, 0, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}