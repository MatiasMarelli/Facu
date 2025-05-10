#include <omp.h>
#include <stdio.h>


int main(){
  int sum=0;
  int val=1;

  #pragma omp parallel 
  {
    #pragma omp for reduction (+:sum)
    for(int i=0;i<1000;++i){
      sum =sum + val;
    }
  }
  printf("Sum: %d\n",sum);


  return 0;
}