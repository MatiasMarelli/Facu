#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#define threads 1
#define numerodemierda 500000000

volatile double suma = 0;
pthread_mutex_t mut =PTHREAD_MUTEX_INITIALIZER;
volatile int idx=0;
double* arr;

double* rand_arr(){
    double* arr=malloc(sizeof(double)*numerodemierda);
    for (int i = 0; i < numerodemierda; i++){
        arr[i] = 1;
    } 
    return arr;
}


void* sum_arr(void* ptr){
  while (idx < numerodemierda){
    pthread_mutex_lock(&mut);
    suma += arr[idx];
    idx++;
    pthread_mutex_unlock(&mut);
  }
  
}


int main(){
  
  arr=rand_arr();
  pthread_t h [threads];
	
  
  for (int i=0;i<threads;i++){
    pthread_create(&h[i],NULL,sum_arr,NULL);
  }
  for(int i=0;i<threads;i++){
    pthread_join(h[i],NULL);
  }
  
  

  printf("Suma:%f\n",suma);


  free(arr);
  return 0;
}