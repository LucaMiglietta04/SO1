/*Implementar un programa donde el usuario de un valor de entrada y el proceso root
envíe el valor al siguiente proceso. Luego, cada proceso envía el valor al siguiente
proceso (por ID de proceso) hasta haber pasado por todos los procesos*/

#include <stdio.h>
#include <string.h>
#include <mpi.h>

void main( int argc, char** argv){
    int num_procs, my_id, value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if (my_id==0){
        fflush(stdout);
        scanf("%d", &value);        
        MPI_Send(&value, 1 ,MPI_INT, my_id+1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&value, 1 , MPI_INT, my_id-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Recibí el valor %d, del proceso %d\n", value, my_id);
        if (my_id < (num_procs - 1)){ MPI_Send(&value, 1, MPI_INT, (my_id + 1), 0 ,MPI_COMM_WORLD);}
    }
    MPI_Finalize();
}