#include <stdio.h>
#include <mpi.h>

void main(int argc, char** argv){
  int num_procs,my_id;
  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD,&my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  
 

  

  MPI_Finalize();
}