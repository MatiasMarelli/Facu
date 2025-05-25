#include <omp.h>
#include <stdio.h>
#include <unistd.h>

void pr1(){
  for(int i=0;i<3;i++){
    printf("%d\n",i);
    sleep(2);
  }
}

void pr2(){
  for(int i=0;i<6;i++){
    printf("%d\n",i);
    sleep(1);
  }
}


int main(){

  #pragma omp parallel sections 
  {
    #pragma omp section
    pr1();
    #pragma omp section
    pr2();
  }


  return 0;
}