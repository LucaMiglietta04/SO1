#include <stdio.h>
#include <string.h>
#include <mpi.h>

void main( int argc, char** argv){
   int process_Rank, size_Of_Comm;
    int distro_Array[8] = {0};
    int scattered_Data[2];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Comm);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);
    if (process_Rank==0){
      for (int i =0; i<8; i++){
         distro_Array[i]=i;
      }
    }
    MPI_Scatter(&distro_Array, 1, MPI_INT, &scattered_Data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i=0; i<2; i++){
      printf("Process %d has received: %d \n", process_Rank, scattered_Data[i]);
    }
    
    MPI_Finalize();
    }