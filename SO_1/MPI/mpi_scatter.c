#include <stdio.h>
#include <mpi.h>

void main(int argc, char** argv){
  int process_rank,size_of_Comm;
  int distro_Array [4] = {39 ,72,129,42};
  int scattered_Data;
  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD,&process_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_Comm);
  
  MPI_Scatter( &distro_Array , 1 , MPI_INT , &scattered_Data , 1 , MPI_INT , 0 , MPI_COMM_WORLD);

  printf("Process %d has received: %d\n",process_rank,scattered_Data);

  MPI_Finalize();
}