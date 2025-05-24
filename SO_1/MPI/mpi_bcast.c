#include <stdio.h>
#include <mpi.h>

void main(int argc, char** argv){
  int num_procs,my_id,value;
  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD,&my_id);
  
  if(my_id ==0){
    printf("ENTER NUMBER TO BROADCAST:\n");
    scanf("%d",&value);
  }else{
    printf("Process %d: Before MPI_Bcast, value is %d\n",my_id,value);
  }
  MPI_Bcast( &value , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
  
  printf("Process %d: after MPI_Bcast, value is %d\n",my_id,value);

  
  MPI_Finalize();
}