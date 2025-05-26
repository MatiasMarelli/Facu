#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include "timing.h"
#include <stdio.h>


#define N 500000000
#define T 6
double min=0;
double* arr;

double* rand_arr(){
    double* arr=malloc(sizeof(double)*N);
    for (int i = 0; i < N; i++){
      arr[i]= ((-1)^(rand()%2))*((rand()%100240)*1.0)/((rand()%10*1.0)+1);
    } 
    return arr;
  }

void print (){
  for(int i=0;i<N;i++){

    printf("%f ",arr[i]);
  }
  printf("\n");
}


void min_search(int id){
  
  unsigned long long int start=id*(N/T);
  unsigned long long int idx=0;
  for(;idx<N/T;idx++){
    #pragma omp critical
    {
      if(arr[idx+start]<min){
        min =arr[idx+start];
        
      }
    }
    
  }
}

void min_arr(){
  #pragma omp parallel num_threads(T)
  {
    int id = omp_get_thread_num();
    //printf("ID:%d\n",id);
    min_search(id);
  }
  printf("Min:%f\n",min);
}

int main(){
  arr=rand_arr(); 
  printf("Termine de hacer el arrroz\n");
  min=arr[0];
  float time;
  TIME_void(min_arr(),&time);

  free(arr);
  return 0;
}