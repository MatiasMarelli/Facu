
#include <omp.h>
#include <stdio.h>

#define Visitas 1000000

int visitas =0;

void molinete(){

  for(int i=0; i<Visitas/2;i++){
    #pragma omp critical
    {
      visitas++;
    }
  }
}


int main (){

  #pragma omp parallel num_threads(2)
  {
    molinete();
  }

  printf("Visitas:%d\n",visitas);

  return 0;
}