#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define threads 12
#define numerodemierda 500000000

long double suma = 0;

double* arr;

double* rand_arr(){
    double* arr=malloc(sizeof(double)*numerodemierda);
    for (int i = 0; i < numerodemierda; i++){
        arr[i]= ((rand()%1000)*1.0)/((rand()%15*1.0)+1);
    } 
    return arr;
}

void print (){
  for(int i=0;i<numerodemierda;i++){

    printf("%f ",arr[i]);
  }
  printf("\n");
}


int main(){
  
  arr=rand_arr();
	//print();

  #pragma omp parallel num_threads(threads)
  {
    #pragma omp for reduction (+:suma)
    for(int i=0; i<numerodemierda;i++){
        
        {
          suma += arr[i];
        }
    }
  }
  
  
  printf("Suma:%Lf\n",suma);

  free(arr);
  return 0;
}