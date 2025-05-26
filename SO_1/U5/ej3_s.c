#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "timing.h"
#include <stdio.h>


#define N 500000000
#define T 1
double min=0;
pthread_mutex_t mut= PTHREAD_MUTEX_INITIALIZER;
double* arr;

double* rand_arr(){
    printf("%p\n",malloc(sizeof(double)*N));
    
    for (long i = 0; i < N; i++){
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


void* min_search(void* i){
  int id=i-(void*)0;
  unsigned long long int start=id*(N/T);
  unsigned long long int idx=0;
  for(;idx<N/T;idx++){
    //pthread_mutex_lock(&mut);
    if(arr[idx+start]<min){
      min =arr[idx+start];
      
    }
    //pthread_mutex_unlock(&mut);
  }
  return NULL;
}

/* void min_arr(){
  #pragma omp parallel num_threads(T)
  {
    int id = omp_get_thread_num();
    //printf("ID:%d\n",id);
    min_search(id);
  }
  printf("Min:%f\n",min);
  } */

  void fun(pthread_t* h){
    for(int i=0;i<T;i++){
     //printf("idx:%d\n",i);
     pthread_create(&h[i],NULL,min_search,i);
    }
    for(int i=0;i<T;i++){
      pthread_join(h[i],NULL);
    }
  }
 
 int main(){
 /*  clock_t inicio,fin;
  double cpu_time_used; */
   pthread_t h[T];
   arr=rand_arr(); 
   printf("Termine de hacer el arrroz\n");
   min=arr[0];
   printf("MIN:%f\n",min);
  float time;
  TIME_void(min_search(0),&time);
    printf("Min:%f\n",min);
    free(arr);

  return 0;
  
}