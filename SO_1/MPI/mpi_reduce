#include <stdio.h>
#include <mpi.h>

void main(int argc, char** argv){
  int process_rank,size_of_Comm;
  int distro_Array [4] = {39 ,72,129,42};
  int scattered_Data;
  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD,&process_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_Comm);
  
  int reduce_result =0;

  MPI_Reduce( &process_rank , &reduce_result , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD);

  if(process_rank ==0){
    printf("The sum is:%d\n", reduce_result);
  }

  //printf("Process %d has received: %d\n",process_rank,scattered_Data);

  MPI_Finalize();
}