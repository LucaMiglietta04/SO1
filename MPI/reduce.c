#include <stdio.h>
#include <string.h>
#include <mpi.h>

void main( int argc, char** argv){
    // Get my rank
    int my_rank, root_rank;
    root_rank = 0;
    MPI_Init( &argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int reduction_result = 0;
    MPI_Reduce(&my_rank, &reduction_result, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);
    if(my_rank == root_rank)
    {
    printf("The sum of all ranks is %d.\n", reduction_result);
    }
    MPI_Finalize();
}